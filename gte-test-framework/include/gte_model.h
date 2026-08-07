#ifndef GTE_MODEL_H
#define GTE_MODEL_H

#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include <cstring>

namespace gte {

// Union types for register access matching GTE hardware behavior
struct RegByte {
    int32_t l;
    int32_t h;
    int32_t h2;
    int32_t h3;
};

struct RegWord {
    int32_t l;
    int32_t h;
};

struct RegSignedWord {
    int32_t l;
    int32_t h;
};

struct RegDoubleWord {
    int32_t d;
};

struct RegSignedLong {
    int64_t sd;
};

// Data register union (cop2r0-31)
struct RegData {
    union {
        int32_t    d;
        RegByte    b;
        RegWord    w;
        RegSignedWord sw;
        int64_t    sd;
    };
};

// Control register union (cop2r32-63)
struct RegControl {
    union {
        int32_t    d;
        RegByte    b;
        RegWord    w;
        RegSignedWord sw;
        int64_t    sd;
    };
};

// GTE instruction field extraction (gte.txt spec)
struct GTEFields {
    int32_t command;  // bits 0-5: real GTE command
    int32_t fakeop;   // bits 20-24: fake GTE command number
    int32_t sf;       // bit 19: shift fraction
    int32_t mx;       // bits 17-18: matrix select
    int32_t v;        // bits 15-16: vector select
    int32_t cv;       // bits 13-14: translation vector select
    int32_t lm;       // bit 10: saturate mode

    // Extract fields from a raw GTE opcode
    static GTEFields from_opcode(uint32_t opcode) {
        GTEFields f;
        f.command = opcode & 0x3F;
        f.fakeop  = (opcode >> 20) & 0x1F;
        f.sf      = (opcode >> 19) & 1;
        f.mx      = (opcode >> 17) & 3;
        f.v       = (opcode >> 15) & 3;
        f.cv      = (opcode >> 13) & 3;
        f.lm      = (opcode >> 10) & 1;
        return f;
    }

    // Assemble opcode from fields (inverse of from_opcode)
    static uint32_t to_opcode(const GTEFields& f) {
        return static_cast<uint32_t>(
            f.command |
            (f.fakeop << 20) |
            (f.sf << 19) |
            (f.mx << 17) |
            (f.v << 15) |
            (f.cv << 13) |
            (f.lm << 10)
        );
    }
};

// Register state model - indexed by register number
// Data registers: d0-d31 (cop2r0-31)
// Control registers: c0-c31 (cop2r32-63)
struct RegisterState {
    // Data registers d0-d31 (simplified int32_t view)
    int32_t data[32];
    // Control registers c0-c31 (simplified int32_t view)
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

// Forward declaration - defined in gte_stub.h
class CPUState;

// Apply register state to a CPUState (emulator interface)
void apply_to_cpu(const RegisterState& state, CPUState& cpu);

// Load register state from a CPUState (emulator interface)
void load_from_cpu(RegisterState& state, const CPUState& cpu);

struct TestCase {
    std::string name;
    int32_t command;
    int32_t fakeop;
    int32_t sf;
    int32_t mx;
    int32_t v;
    int32_t cv;
    int32_t lm;
    // Pre-assembled GTE opcode from test data (if present)
    // When present, test framework uses this directly
    // When absent, framework assembles from command, fakeop, sf, mx, v, cv, lm
    int32_t raw_opcode;
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

    static int32_t saturate_16_signed(int32_t value);
    static int32_t saturate_16_unsigned(int32_t value);
    static int32_t saturate_32(int32_t value);
    static int32_t shift_result(int32_t value, int32_t sf);
};

}

#endif
