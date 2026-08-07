#ifndef GTE_DUMMY_API_H
#define GTE_DUMMY_API_H

#include "gte_model.h"
#include <string>
#include <functional>
#include <map>
#include <vector>

namespace gte {

// GTEStub - interface for porting GTE implementation to a PSX emulator
//
// Design principles:
// 1. Accepts raw 32-bit GTE opcode (bits 24-0 of COP2 instruction)
// 2. Decodes instruction fields in place using pcsx-compatible bit masks
// 3. Provides CPUState interface for emulator register access
// 4. Easy to port: implement CPUState and register command functions
//
// Porting to an emulator:
//   1. Create a CPUState implementation that wraps your emulator's CP2D/CP2C
//   2. Register command functions that access the CPUState directly
//   3. Call execute_command(raw_opcode, cpu_state) for each GTE instruction
//
// Example port:
//   class MyGTECPUState : public CPUState {
//       int32_t get_data_d(int n) const override { return psxRegs.CP2D.p[n].d; }
//       void set_data_d(int n, int32_t v) override { psxRegs.CP2D.p[n].d = v; }
//       // ... implement all virtual methods
//   };
//
//   GTEStub stub;
//   stub.register_command(0x01, [](int32_t opcode, CPUState& cpu) {
//       // RTPS implementation - access cpu.get_data_d(), cpu.get_control_d(), etc.
//       // Decode fields: auto fields = GTEFields::decode(opcode);
//       // int sf = fields.sf; int mx = fields.mx; etc.
//   });
//
//   // Execute:
//   MyGTECPUState cpu_state;
//   stub.execute_command(raw_gte_opcode, cpu_state);

class GTEStub {
public:
    // Command function type - receives raw opcode and CPU state
    // The raw opcode allows the function to decode fields in place (pcsx style)
    using CommandFunc = std::function<void(int32_t opcode, CPUState& cpu)>;

    GTEStub();
    ~GTEStub();

    // Execute a GTE command
    // opcode: raw 25-bit GTE opcode (bits 24-0 of COP2 instruction)
    // cpu: CPU state providing access to CP2D/CP2C registers
    void execute_command(int32_t opcode, CPUState& cpu);

    // Get decoded instruction fields (pcsx-compatible extraction)
    GTEFields decode_fields(int32_t opcode) const;

    // Register/unregister command implementations
    void register_command(int32_t opcode, CommandFunc func);
    void unregister_command(int32_t opcode);

    std::vector<int32_t> get_supported_commands() const;

private:
    std::map<int32_t, CommandFunc> commands_;
};

// GTECPUState - simplified CPUState for standalone testing (non-emulator)
// Wraps a RegisterState for use with GTEStub in test scenarios
class GTECPUState : public CPUState {
public:
    GTECPUState();
    explicit GTECPUState(const RegisterState& state);

    void set_state(const RegisterState& state) const;
    RegisterState get_state() const;

    // CPUState interface - provides direct access to register data
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

// Forward declaration for registering dummy commands
void register_dummy_commands(GTEStub& stub);

}

#endif
