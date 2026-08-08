#if defined(_WIN32)
#include <windows.h>
#endif

#include "pcsx_gte_stub.h"
#include "pcsx_gte_cpu_state.h"
#include "test_framework.h"
#include "json_parser.h"
#include "gte_model.h"
#include "psxcommon.h"
#include "gte.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#if defined(_WIN32)
#undef R
#undef G
#undef B
#undef CODE
#undef CP2D
#undef CP2C
#define WIN32_FIND
#else
#include <dirent.h>
#endif

// Global state defined in pcsx_globals.c

static void dummy_gpu_vertex(s32 sx, s32 sy, s32 ix, s32 iy, s32 iz) {
    (void)sx; (void)sy; (void)ix; (void)iy; (void)iz;
}

namespace gte {

static std::string format_hex(int32_t value) {
    char buf[16];
    snprintf(buf, sizeof(buf), "0x%08X", (unsigned int)(uint32_t)value);
    return std::string(buf);
}

static int run_single_test(const TestCase& test, PCSXGTECPUState& cpu_state) {
    cpu_state.set_state(test.initial);
    cpu_state.sync_to_psx_regs();

    uint32_t opcode = assemble_gte_opcode(test.command, test.fakeop, test.sf, test.mx, test.v, test.cv, test.lm);
    psxRegs_global.code = opcode;

    GTEFields fields = GTEFields::from_opcode(opcode);

    switch (fields.command) {
        case 0x01: gteRTPS(); break;
        case 0x02: gteRTPT(); break;
        case 0x06: gteNCLIP(); break;
        case 0x0C: gteOP(); break;
        case 0x0E: gteNCDS(); break;
        case 0x10: gteDPCS(); break;
        case 0x11: gteINTPL(); break;
        case 0x12: gteMVMVA(); break;
        case 0x1B: gteNCCS(); break;
        case 0x1E: gteNCS(); break;
        case 0x28: gteSQR(); break;
        case 0x29: gteDCPL(); break;
        case 0x2D: gteAVSZ3(); break;
        case 0x2E: gteAVSZ4(); break;
        case 0x3D: gteGPF(); break;
        case 0x3E: gteGPL(); break;
        case 0x14: gteCDP(); break;
        case 0x16: gteNCDT(); break;
        case 0x20: gteNCT(); break;
        case 0x2A: gteDPCT(); break;
        case 0x3F: gteNCCT(); break;
        case 0x1C: gteCC(); break;
        default:
            std::cerr << "Unknown GTE command: 0x" << std::hex << (int)fields.command << std::dec << "\n";
            return -1;
    }

    cpu_state.sync_from_psx_regs();
    RegisterState actual = cpu_state.get_state();

    bool passed = (actual == test.final_state);
    if (!passed) {
        std::cerr << "FAIL: " << test.name << "\n";
        for (int i = 0; i < 32; i++) {
            if (test.final_state.data[i] != 0 && actual.data[i] != test.final_state.data[i]) {
                char buf[256];
                snprintf(buf, sizeof(buf), "d%d: expected %s got %s", i + 1,
                         format_hex(test.final_state.data[i]).c_str(),
                         format_hex(actual.data[i]).c_str());
                std::cerr << "  " << buf << "\n";
            }
        }
        for (int i = 0; i < 32; i++) {
            if (test.final_state.control[i] != 0 && actual.control[i] != test.final_state.control[i]) {
                char buf[256];
                snprintf(buf, sizeof(buf), "c%d: expected %s got %s", i + 1,
                         format_hex(test.final_state.control[i]).c_str(),
                         format_hex(actual.control[i]).c_str());
                std::cerr << "  " << buf << "\n";
            }
        }
    } else {
        std::cout << "PASS: " << test.name << "\n";
    }

    return passed ? 0 : 1;
}

static std::vector<std::string> list_json_files(const std::string& dir) {
    std::vector<std::string> files;

#if defined(_WIN32)
    std::string pattern = dir + "/*.json";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Cannot open directory: " << dir << "\n";
        return files;
    }
    do {
        files.push_back(dir + "/" + std::string(fd.cFileName));
    } while (FindNextFileA(h, &fd) != 0);
    FindClose(h);
#else
    DIR* d = opendir(dir.c_str());
    if (!d) {
        std::cerr << "Error: Cannot open directory: " << dir << "\n";
        return files;
    }
    struct dirent* entry;
    while ((entry = readdir(d)) != NULL) {
        const char* name = entry->d_name;
        size_t len = strlen(name);
        if (len > 5 && strcmp(name + len - 5, ".json") == 0) {
            files.push_back(dir + "/" + std::string(name));
        }
    }
    closedir(d);
#endif

    return files;
}

static int run_all_tests(const std::string& test_dir) {
    std::vector<std::string> files = list_json_files(test_dir);
    if (files.empty()) {
        std::cerr << "No test files found in: " << test_dir << "\n";
        return 1;
    }

    int total = 0;
    int passed = 0;
    int failed = 0;

    for (const auto& filepath : files) {
        std::cout << "\nLoading test file: " << filepath << "\n";

        // Read JSON file
        FILE* f = fopen(filepath.c_str(), "r");
        if (!f) {
            std::cerr << "  Cannot open file: " << filepath << "\n";
            continue;
        }
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, 0, SEEK_SET);
        std::string json_str(size, '\0');
        fread(&json_str[0], 1, size, f);
        fclose(f);

        JsonPtr root = JsonParser::parse_array(json_str);
        JsonArray tests = get_array(root);

        for (const auto& test_val : tests) {
            JsonObject obj = get_object(test_val);
            TestCase tc;
            tc.name = get_string(get_field(obj, "name"));
            tc.command = (int32_t)get_int_field(obj, "command");
            tc.fakeop = (int32_t)get_int_field(obj, "fakeop");
            tc.sf = (int32_t)get_int_field(obj, "sf");
            tc.mx = (int32_t)get_int_field(obj, "mx");
            tc.v = (int32_t)get_int_field(obj, "v");
            tc.cv = (int32_t)get_int_field(obj, "cv");
            tc.lm = (int32_t)get_int_field(obj, "lm");

            JsonObject init_obj = get_object(get_field(obj, "initial"));
            JsonObject final_obj = get_object(get_field(obj, "final"));

            for (int i = 0; i < 32; i++) {
                char key[8];
                snprintf(key, sizeof(key), "d%d", i + 1);
                JsonPtr val = get_field(init_obj, key);
                if (val) tc.initial.data[i] = (int32_t)get_int(val);

                snprintf(key, sizeof(key), "c%d", i + 1);
                val = get_field(init_obj, key);
                if (val) tc.initial.control[i] = (int32_t)get_int(val);
            }
            for (int i = 0; i < 32; i++) {
                char key[8];
                snprintf(key, sizeof(key), "d%d", i + 1);
                JsonPtr val = get_field(final_obj, key);
                if (val) tc.final_state.data[i] = (int32_t)get_int(val);

                snprintf(key, sizeof(key), "c%d", i + 1);
                val = get_field(final_obj, key);
                if (val) tc.final_state.control[i] = (int32_t)get_int(val);
            }

            total++;
            int result = run_single_test(tc, PCSXGTECPUState());
            if (result == 0) passed++;
            else failed++;
        }
    }

    std::cout << "\n========================================\n";
    std::cout << "   PCSX GTE Test Framework Summary\n";
    std::cout << "========================================\n";
    std::cout << "Total tests:  " << total << "\n";
    std::cout << "Passed:       " << passed << "\n";
    std::cout << "Failed:       " << failed << "\n";
    if (total > 0) {
        double rate = (double)passed / total * 100.0;
        char buf[32];
        snprintf(buf, sizeof(buf), "%.1f%%", rate);
        std::cout << "Pass rate:    " << buf << "\n";
    }
    std::cout << "========================================\n";

    return failed > 0 ? 1 : 0;
}

}

int main(int argc, char* argv[]) {
    std::string test_dir = "./v1";

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: gte-pcsx-runner [options] [test_dir]\n"
                      << "Options:\n"
                      << "  -h, --help     Show help\n"
                      << "  -d, --dir <d>  Test directory\n"
                      << "  -s, --summary  Show only summary\n"
                      << "  -v, --verbose  Verbose output\n";
            return 0;
        }
        if ((arg == "-d" || arg == "--dir") && i + 1 < argc) {
            test_dir = argv[i + 1];
            i++;
        }
    }

    std::cout << "PCSX GTE Test Framework Runner\n";
    std::cout << "==============================\n";
    std::cout << "Test directory: " << test_dir << "\n";

    g_Config.Widescreen = Widescreen;
    GPU_addVertex = dummy_gpu_vertex;
    memset(&psxRegs_global, 0, sizeof(psxRegs_global));

    return gte::run_all_tests(test_dir);
}
