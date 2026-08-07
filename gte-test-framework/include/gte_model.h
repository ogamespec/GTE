#ifndef GTE_MODEL_H
#define GTE_MODEL_H

#include <string>
#include <map>
#include <cstdint>

namespace gte {

struct RegisterState {
    // Data registers d0-d31
    int32_t data[32];
    // Control registers c0-c31
    int32_t control[32];

    RegisterState();
    void clear();
    bool operator==(const RegisterState& other) const;
    bool operator!=(const RegisterState& other) const;

    int32_t get_data(int index) const;
    int32_t get_control(int index) const;
    void set_data(int index, int32_t value);
    void set_control(int index, int32_t value);
};

struct TestCase {
    std::string name;
    std::string command;
    int32_t sf;
    int32_t mx;
    int32_t v;
    int32_t cv;
    int32_t lm;
    RegisterState initial;
    RegisterState final_state;
};

struct TestFile {
    std::string filename;
    std::vector<TestCase> tests;
};

class GTEModel {
public:
    RegisterState get_state() const;
    void set_state(const RegisterState& state);

    // Saturation helpers
    static int32_t saturate_16_signed(int32_t value);
    static int32_t saturate_16_unsigned(int32_t value);
    static int32_t saturate_32(int32_t value);

    // Fixed-point shift
    static int32_t shift_result(int32_t value, int32_t sf);
};

}

#endif
