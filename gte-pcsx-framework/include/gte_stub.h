#ifndef GTE_GTE_STUB_H
#define GTE_GTE_STUB_H

#include "gte_model.h"
#include <cstdint>
#include <functional>
#include <map>
#include <vector>

namespace gte {

// Assemble GTE opcode from test properties according to GTE specification (gte.txt).
//
// GTE Command Encoding (COP2 imm25 opcodes):
//   31-25  Must be 0100101b for "COP2 imm25" instructions
//   24-20  Fake GTE Command Number (00h..1Fh) (ignored by hardware)
//   19     sf - Shift Fraction (0=No fraction, 1=12bit fraction)
//   18-17  MVMVA Multiply Matrix (0=Rotation, 1=Light, 2=Color, 3=Reserved)
//   16-15  MVMVA Multiply Vector (0=V0, 1=V1, 2=V2, 3=IR/long)
//   14-13  MVMVA Translation Vector (0=TR, 1=BK, 2=FC, 3=None)
//   12-11  Always zero (ignored by hardware)
//   10     lm - Saturate IR1,IR2,IR3 (0=To -8000h..+7FFFh, 1=To 0..+7FFFh)
//   9-6    Always zero (ignored by hardware)
//   5-0    Real GTE Command Number (00h..3Fh) (used by hardware)
//
// Parameters:
//   command - Real GTE command opcode (bits 0-5, 0x00..0x3F)
//   fakeop  - Fake GTE command number (bits 20-24, 0x00..0x1F)
//   sf      - Shift fraction flag (bit 19, 0 or 1)
//   mx      - Matrix select (bits 17-18, 0=Rotation, 1=Light, 2=Color)
//   v       - Vector select (bits 15-16, 0=V0, 1=V1, 2=V2, 3=IR)
//   cv      - Translation vector select (bits 13-14, 0=TR, 1=BK, 2=FC, 3=None)
//   lm      - Saturate mode (bit 10, 0=signed, 1=unsigned)
//
// Returns: 32-bit GTE opcode with COP2 imm25 format
uint32_t assemble_gte_opcode(int32_t command, int32_t fakeop,
                              int32_t sf, int32_t mx, int32_t v,
                              int32_t cv, int32_t lm);

// CPUState interface - emulators implement this to provide GTE register access
// Registers are indexed by number (d0-d31 for data, c0-c31 for control)
class CPUState {
public:
    virtual ~CPUState() = default;

    // Data register access (d0-d31)
    virtual int32_t get_data_d(int n) const = 0;
    virtual void set_data_d(int n, int32_t value) = 0;

    // Control register access (c0-c31)
    virtual int32_t get_control_d(int n) const = 0;
    virtual void set_control_d(int n, int32_t value) = 0;

    // Raw 32-bit register access (same as d/ctrl but for union-based types)
    virtual int32_t get_data_raw(int n) const = 0;
    virtual void set_data_raw(int n, int32_t value) = 0;
    virtual int32_t get_control_raw(int n) const = 0;
    virtual void set_control_raw(int n, int32_t value) = 0;
};

// GTEStub - interface for testing GTE implementations
//
// Usage:
//   1. Implement CPUState to wrap your emulator's CP2D/CP2C registers
//   2. Register command functions that access registers by index
//   3. Call gte_execute(opcode, cpu) for each GTE instruction
//
// Example porting to an emulator:
//   class MyCPUState : public CPUState {
//       int32_t get_data_d(int n) const override { return psxRegs.CP2D.p[n].d; }
//       void set_data_d(int n, int32_t v) override { psxRegs.CP2D.p[n].d = v; }
//       int32_t get_control_d(int n) const override { return psxRegs.CP2C.p[n].d; }
//       void set_control_d(int n, int32_t v) override { psxRegs.CP2C.p[n].d = v; }
//       int32_t get_data_raw(int n) const override { return get_data_d(n); }
//       void set_data_raw(int n, int32_t v) override { set_data_d(n, v); }
//       int32_t get_control_raw(int n) const override { return get_control_d(n); }
//       void set_control_raw(int n, int32_t v) override { set_control_d(n, v); }
//   };
//
//   GTEStub stub;
//   stub.register_command(0x01, [](uint32_t opcode, CPUState& cpu) {
//       auto fields = GTEFields::from_opcode(opcode);
//       // Access registers by index: cpu.get_data_d(0) for d0, etc.
//   });
//   MyCPUState cpu_state;
//   stub.gte_execute(assemble_gte_opcode(1, 0, 0, 0, 0, 0, 0), cpu_state);

class GTEStub {
public:
    using CommandFunc = std::function<void(uint32_t opcode, CPUState& cpu)>;

    GTEStub();
    ~GTEStub();

    // Execute a GTE command with the given opcode
    // opcode: GTE opcode assembled via assemble_gte_opcode()
    // cpu: CPU state providing access to GTE registers (indexed 0-31)
    void gte_execute(uint32_t opcode, CPUState& cpu);

    // Get decoded instruction fields from an opcode
    GTEFields decode_fields(uint32_t opcode) const;

    // Register/unregister command implementations
    void register_command(int32_t opcode, CommandFunc func);
    void unregister_command(int32_t opcode);

    std::vector<int32_t> get_supported_commands() const;

private:
    std::map<int32_t, CommandFunc> commands_;
};

// GTECPUState - CPUState implementation for standalone testing
// Wraps a RegisterState for use with GTEStub in test scenarios
class GTECPUState : public CPUState {
public:
    GTECPUState();
    explicit GTECPUState(const RegisterState& state);

    void set_state(const RegisterState& state) const;
    RegisterState get_state() const;

    // CPUState interface - registers indexed by number (d0-d31, c0-c31)
    int32_t get_data_d(int n) const override;
    void set_data_d(int n, int32_t value) override;
    int32_t get_control_d(int n) const override;
    void set_control_d(int n, int32_t value) override;
    int32_t get_data_raw(int n) const override;
    void set_data_raw(int n, int32_t value) override;
    int32_t get_control_raw(int n) const override;
    void set_control_raw(int n, int32_t value) override;

private:
    mutable RegisterState state_;
};

// Register all GTE commands with dummy (no-op) implementations
void register_dummy_commands(GTEStub& stub);

}

#endif
