#include "test_framework.h"
#include "json_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace gte {

TestFramework::TestFramework() {}

TestFile TestFramework::load_test_file(const std::string& filepath) {
    TestFile test_file;
    test_file.filename = filepath;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open test file: " << filepath << "\n";
        return test_file;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json = buffer.str();
    file.close();

    JsonPtr test_cases_ptr = JsonParser::parse_array(json);
    if (!test_cases_ptr || test_cases_ptr->type != JsonValue::ARRAY) {
        std::cerr << "Error: Expected JSON array in " << filepath << "\n";
        return test_file;
    }

    const JsonArray& test_cases = test_cases_ptr->arr_val;

    for (const auto& tc_val : test_cases) {
        TestCase tc;
        auto obj = get_object(tc_val);
        if (obj.empty()) {
            continue;
        }

        tc.name = get_string(get_field(obj, "name"), "unnamed");
        tc.command = static_cast<int32_t>(get_int_field(obj, "command", 0));
        tc.fakeop = static_cast<int32_t>(get_int_field(obj, "fakeop", 0));
        tc.sf = get_int_field(obj, "sf", 0);
        tc.mx = get_int_field(obj, "mx", 0);
        tc.v = get_int_field(obj, "v", 0);
        tc.cv = get_int_field(obj, "cv", 0);
        tc.lm = get_int_field(obj, "lm", 0);

        // Assemble GTE opcode from test properties per GTE spec (gte.txt)
        uint32_t opcode = assemble_gte_opcode(tc.command, tc.fakeop, tc.sf, tc.mx, tc.v, tc.cv, tc.lm);

        auto initial_ptr = get_field(obj, "initial");
        auto final_ptr = get_field(obj, "final");
        auto initial_obj = get_object(initial_ptr);
        auto final_obj = get_object(final_ptr);

        for (int i = 0; i <= 31; ++i) {
            std::string key = "d" + std::to_string(i);
            auto it = initial_obj.find(key);
            if (it != initial_obj.end() && it->second) {
                tc.initial.set_data(i, static_cast<int32_t>(get_int(it->second)));
            }
            it = final_obj.find(key);
            if (it != final_obj.end() && it->second) {
                tc.final_state.set_data(i, static_cast<int32_t>(get_int(it->second)));
            }
        }

        for (int i = 0; i <= 31; ++i) {
            std::string key = "c" + std::to_string(i);
            auto it = initial_obj.find(key);
            if (it != initial_obj.end() && it->second) {
                tc.initial.set_control(i, static_cast<int32_t>(get_int(it->second)));
            }
            it = final_obj.find(key);
            if (it != final_obj.end() && it->second) {
                tc.final_state.set_control(i, static_cast<int32_t>(get_int(it->second)));
            }
        }

        test_file.tests.push_back(tc);
    }

    return test_file;
}

TestResult TestFramework::run_test(const TestCase& test, GTEStub& stub) {
    TestResult result;
    result.test_name = test.name;
    result.command = static_cast<int32_t>(test.command);
    result.passed = true;

    // Assemble GTE opcode from test properties per GTE spec (gte.txt)
    uint32_t opcode = assemble_gte_opcode(test.command, test.fakeop, test.sf, test.mx, test.v, test.cv, test.lm);

    // Create CPU state from initial register state
    GTECPUState cpu_state(test.initial);

    // Execute command via GTEStub::gte_execute
    stub.gte_execute(opcode, cpu_state);

    // Get actual state and compare with expected final state
    RegisterState actual_state = cpu_state.get_state();

    result.passed = compare_registers(actual_state, test.final_state, result.mismatches);

    if (!result.passed) {
        result.failure_reason = "Register mismatch detected";
    }

    return result;
}

bool TestFramework::compare_registers(const RegisterState& actual, const RegisterState& expected,
                                       std::map<std::string, std::pair<int32_t, int32_t>>& mismatches) {
    bool all_match = true;

    for (int i = 0; i <= 31; ++i) {
        int32_t exp_val = expected.get_data(i);
        int32_t act_val = actual.get_data(i);
        if (exp_val != 0 && exp_val != act_val) {
            std::string key = "d" + std::to_string(i);
            mismatches[key] = {act_val, exp_val};
            all_match = false;
        }
    }

    for (int i = 0; i <= 31; ++i) {
        int32_t exp_val = expected.get_control(i);
        int32_t act_val = actual.get_control(i);
        if (exp_val != 0 && exp_val != act_val) {
            std::string key = "c" + std::to_string(i);
            mismatches[key] = {act_val, exp_val};
            all_match = false;
        }
    }

    return all_match;
}

void TestFramework::run_test_file(const std::string& filepath, GTEStub& stub) {
    TestFile test_file = load_test_file(filepath);

    for (const auto& test : test_file.tests) {
        TestResult result = run_test(test, stub);
        result.filename = test_file.filename;
        results_.push_back(result);
    }
}

void TestFramework::run_all_tests(const std::string& test_dir, GTEStub& stub) {
    results_.clear();

    if (!fs::exists(test_dir)) {
        std::cerr << "Error: Test directory does not exist: " << test_dir << "\n";
        return;
    }

    for (const auto& entry : fs::directory_iterator(test_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::string filepath = entry.path().string();
            std::cout << "Loading test file: " << filepath << "\n";
            run_test_file(filepath, stub);
        }
    }
}

void TestFramework::print_summary() const {
    std::cout << "\n========================================\n";
    std::cout << "       GTE Test Framework Summary\n";
    std::cout << "========================================\n";
    std::cout << "Total tests:  " << get_total_count() << "\n";
    std::cout << "Passed:       " << get_passed_count() << "\n";
    std::cout << "Failed:       " << get_failed_count() << "\n";

    if (get_total_count() > 0) {
        double pass_rate = (static_cast<double>(get_passed_count()) / get_total_count()) * 100.0;
        std::cout << "Pass rate:    " << std::fixed << std::setprecision(1) << pass_rate << "%\n";
    }
    std::cout << "========================================\n";
}

void TestFramework::print_detailed_report() const {
    std::cout << "\n========================================\n";
    std::cout << "       Detailed Test Report\n";
    std::cout << "========================================\n\n";

    std::string current_file;
    for (const auto& result : results_) {
        if (result.filename != current_file) {
            current_file = result.filename;
            std::cout << "--- " << current_file << " ---\n";
        }

        std::string status = result.passed ? "[PASS]" : "[FAIL]";
        std::cout << "  " << status << " " << result.test_name << " (cmd: 0x" << std::hex << std::setw(2) << std::setfill('0') << (result.command & 0x3F) << " op: 0x" << std::setw(8) << std::setfill('0') << result.raw_opcode << std::dec << ")\n";

        if (!result.passed && !result.mismatches.empty()) {
            for (const auto& kv : result.mismatches) {
                std::cout << "         " << kv.first << ": expected 0x" << std::hex << std::setw(8) << std::setfill('0')
                          << kv.second.second << " got 0x" << kv.second.first << "\n";
                std::cout << std::dec;
            }
        }
    }
    std::cout << "\n========================================\n";
}

int TestFramework::get_passed_count() const {
    return static_cast<int>(std::count_if(results_.begin(), results_.end(),
        [](const TestResult& r) { return r.passed; }));
}

int TestFramework::get_failed_count() const {
    return static_cast<int>(std::count_if(results_.begin(), results_.end(),
        [](const TestResult& r) { return !r.passed; }));
}

int TestFramework::get_total_count() const {
    return static_cast<int>(results_.size());
}

std::string TestFramework::format_hex(int32_t value) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << (value & 0xFFFFFFFF);
    return ss.str();
}

}
