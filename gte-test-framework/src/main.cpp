#include "test_framework.h"
#include "gte_stub.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void print_usage(const char* program_name) {
    std::cout << "GTE Test Framework Runner\n";
    std::cout << "\nUsage: " << program_name << " [options] [test_directory]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  -h, --help        Show this help message\n";
    std::cout << "  -d, --dir <dir>   Path to test directory (default: ./v1)\n";
    std::cout << "  -s, --summary     Show only summary (no detailed report)\n";
    std::cout << "  -v, --verbose     Show verbose output\n";
    std::cout << "\nExample:\n";
    std::cout << "  " << program_name << " ./v1\n";
    std::cout << "  " << program_name << " -d ./test_data -s\n";
}

int main(int argc, char* argv[]) {
    std::string test_dir = "./v1";
    bool show_summary_only = false;
    bool verbose = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-d" || arg == "--dir") {
            if (i + 1 < argc) {
                test_dir = argv[i + 1];
                ++i;
            } else {
                std::cerr << "Error: --dir requires a directory path\n";
                return 1;
            }
        } else if (arg == "-s" || arg == "--summary") {
            show_summary_only = true;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else {
            if (fs::exists(arg) && fs::is_directory(arg)) {
                test_dir = arg;
            } else {
                std::cerr << "Warning: Ignoring unknown argument: " << arg << "\n";
            }
        }
    }

    std::cout << "GTE Test Framework Runner\n";
    std::cout << "==========================\n\n";
    std::cout << "Test directory: " << test_dir << "\n";

    // Create GTEStub with dummy implementations
    gte::GTEStub stub;
    gte::register_dummy_commands(stub);

    // Get supported commands
    auto commands = stub.get_supported_commands();
    std::cout << "Registered commands: " << commands.size() << "\n";
    if (verbose) {
        for (const auto& cmd : commands) {
            std::cout << "  - 0x" << std::hex << std::setw(2) << std::setfill('0') << cmd << std::dec << "\n";
        }
    }

    // Run tests
    std::cout << "\nRunning tests...\n\n";
    gte::TestFramework framework;
    framework.run_all_tests(test_dir, stub);

    // Print results
    framework.print_summary();

    if (!show_summary_only) {
        framework.print_detailed_report();
    }

    return framework.get_failed_count() > 0 ? 1 : 0;
}
