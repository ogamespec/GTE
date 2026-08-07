#ifndef GTE_TEST_FRAMEWORK_H
#define GTE_TEST_FRAMEWORK_H

#include "json_parser.h"
#include "gte_model.h"
#include "dummy_api.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>

namespace gte {

struct TestResult {
    std::string test_name;
    int32_t command;
    std::string filename;
    bool passed;
    std::string failure_reason;
    std::map<std::string, std::pair<int32_t, int32_t>> mismatches;
};

class TestFramework {
public:
    TestFramework();

    TestFile load_test_file(const std::string& filepath);
    TestResult run_test(const TestCase& test, DummyGTEAPI& api);
    void run_test_file(const std::string& filepath, DummyGTEAPI& api);
    void run_all_tests(const std::string& test_dir, DummyGTEAPI& api);

    void print_summary() const;
    void print_detailed_report() const;

    int get_passed_count() const;
    int get_failed_count() const;
    int get_total_count() const;

private:
    std::vector<TestResult> results_;

    bool compare_registers(const RegisterState& actual, const RegisterState& expected,
                           std::map<std::string, std::pair<int32_t, int32_t>>& mismatches);
    std::string format_hex(int32_t value);
};

}

#endif
