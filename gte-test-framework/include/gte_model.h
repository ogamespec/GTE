#ifndef GTE_MODEL_H
#define GTE_MODEL_H

#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include <cstring>

namespace gte {

// GTE register access types matching pcsx union-based model
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

// Data register union (CP2D[n]) - matches pcsx psxRegs.CP2D.p[n]
struct RegData {
    union {
        int32_t    d;
        RegByte    b;
        RegWord    w;
        RegSignedWord sw;
        int64_t    sd;
    };
};

// Control register union (CP2C[n]) - matches pcsx psxRegs.CP2C.p[n]
struct RegControl {
    union {
        int32_t    d;
        RegByte    b;
        RegWord    w;
        RegSignedWord sw;
        int64_t    sd;
    };
};

// GTE register names for emulator portability
namespace RegNames {
    // Data registers (d0-d31)
    constexpr int VXY0  = 0;  // d0
    constexpr int VZ0   = 1;  // d1
    constexpr int VXY1  = 2;  // d2
    constexpr int VZ1   = 3;  // d3
    constexpr int VXY2  = 4;  // d4
    constexpr int VZ2   = 5;  // d5
    constexpr int RGB0  = 6;  // d6
    constexpr int OTZ   = 7;  // d7
    constexpr int IR0   = 8;  // d8
    constexpr int IR1   = 9;  // d9
    constexpr int IR2   = 10; // d10
    constexpr int IR3   = 11; // d11
    constexpr int SXY0  = 12; // d12
    constexpr int SXY1  = 13; // d13
    constexpr int SXY2  = 14; // d14
    constexpr int SXYP  = 15; // d15
    constexpr int SZ0   = 16; // d16
    constexpr int SZ1   = 17; // d17
    constexpr int SZ2   = 18; // d18
    constexpr int SZ3   = 19; // d19
    constexpr int RGB1  = 20; // d20
    constexpr int RGB2  = 21; // d21
    constexpr int MAC0  = 24; // d24
    constexpr int MAC1  = 25; // d25
    constexpr int MAC2  = 26; // d26
    constexpr int MAC3  = 27; // d27
    constexpr int IRGB  = 28; // d28
    constexpr int ORGB  = 29; // d29
    constexpr int LZCS  = 30; // d30
    constexpr int LZCR  = 31; // d31

    // Control registers (c0-c31)
    constexpr int RT11  = 0;  // c0
    constexpr int RT12  = 1;  // c1
    constexpr int RT13  = 2;  // c2
    constexpr int RT21  = 3;  // c3
    constexpr int RT22  = 4;  // c4
    constexpr int RT23  = 5;  // c5
    constexpr int RT31  = 6;  // c6
    constexpr int RT32  = 7;  // c7
    constexpr int RT33  = 8;  // c8
    constexpr int TRX   = 9;  // c9
    constexpr int TRY   = 10; // c10
    constexpr int TRZ   = 11; // c11
    constexpr int L11   = 12; // c12
    constexpr int L12   = 13; // c13
    constexpr int L13   = 14; // c14
    constexpr int L21   = 15; // c15
    constexpr int L22   = 16; // c16
    constexpr int L23   = 17; // c17
    constexpr int L31   = 18; // c18
    constexpr int L32   = 19; // c19
    constexpr int L33   = 20; // c20
    constexpr int RBK   = 21; // c21
    constexpr int GBK   = 22; // c22
    constexpr int BBK   = 23; // c23
    constexpr int LR1   = 24; // c24
    constexpr int LR2   = 25; // c25
    constexpr int LR3   = 26; // c26
    constexpr int LG1   = 27; // c27
    constexpr int LG2   = 28; // c28
    constexpr int LB1   = 29; // c29
    constexpr int LB2   = 30; // c30
    constexpr int LB3   = 31; // c31
    constexpr int RFC   = 32; // c32
    constexpr int GFC   = 33; // c33
    constexpr int BFC   = 34; // c34
    constexpr int OFX   = 35; // c35
    constexpr int OFY   = 36; // c36
    constexpr int H     = 37; // c37
    constexpr int DQA   = 38; // c38
    constexpr int DQB   = 39; // c39
    constexpr int ZSF3  = 40; // c40
    constexpr int ZSF4  = 41; // c41
    constexpr int FLAG  = 42; // c42
}

// CPU state interface - emulators implement this to provide GTE register access
class CPUState {
public:
    virtual ~CPUState() = default;

    // Data register access (CP2D) - matches pcsx psxRegs.CP2D.p[n].d
    virtual int32_t get_data_d(int n) const = 0;
    virtual void set_data_d(int n, int32_t value) = 0;

    // Control register access (CP2C) - matches pcsx psxRegs.CP2C.p[n].d
    virtual int32_t get_control_d(int n) const = 0;
    virtual void set_control_d(int n, int32_t value) = 0;

    // Raw 32-bit register access for union-based types
    virtual int32_t get_data_raw(int n) const = 0;
    virtual void set_data_raw(int n, int32_t value) = 0;
    virtual int32_t get_control_raw(int n) const = 0;
    virtual void set_control_raw(int n, int32_t value) = 0;
};

// Register state model - simplified view for test data loading/comparison
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

    // Convert RegisterState to/from CPUState
    void apply_to_cpu(CPUState& cpu) const;
    void load_from_cpu(const CPUState& cpu);
};

// GTE instruction field extraction - matches pcsx decoding
struct GTEFields {
    int32_t command;  // bits 5-0: real GTE command
    int32_t sf;       // bit 19: shift fraction
    int32_t mx;       // bits 17-16: matrix select
    int32_t v;        // bits 15-14: vector select
    int32_t cv;       // bits 13-12: translation vector select
    int32_t lm;       // bit 10: saturate mode
    int32_t fakeop;   // bits 24-20: fake GTE command number

    static GTEFields decode(int32_t gteop) {
        GTEFields f;
        f.command  = gteop & 0x3F;
        f.sf       = (gteop >> 19) & 1;
        f.mx       = (gteop >> 17) & 3;
        f.v        = (gteop >> 15) & 3;
        f.cv       = (gteop >> 13) & 3;
        f.lm       = (gteop >> 10) & 1;
        f.fakeop   = (gteop >> 20) & 0x1F;
        return f;
    }

    // Assemble gteop from fields - same layout as pcsx
    static int32_t assemble(int32_t command, int32_t sf, int32_t mx,
                            int32_t v, int32_t cv, int32_t lm, int32_t fakeop) {
        return command |
               (fakeop << 20) |
               (lm << 10) |
               (cv << 13) |
               (v << 15) |
               (mx << 17) |
               (sf << 19);
    }
};

struct TestCase {
    std::string name;
    int32_t command;
    int32_t fakeop;
    int32_t sf;
    int32_t mx;
    int32_t v;
    int32_t cv;
    int32_t lm;
    // Raw 32-bit GTE opcode (bits 24-0 of the COP2 instruction)
    // When present, fields are extracted from this value using pcsx decoding
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
