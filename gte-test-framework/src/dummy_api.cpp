#include "dummy_api.h"
#include <iostream>
#include <sstream>

namespace gte {

// GTEStub implementation
GTEStub::GTEStub() {}

GTEStub::~GTEStub() {}

GTEFields GTEStub::decode_fields(int32_t opcode) const {
    return GTEFields::decode(opcode);
}

void GTEStub::execute_command(int32_t opcode, CPUState& cpu) {
    auto it = commands_.find(opcode);
    if (it == commands_.end()) {
        std::cerr << "Warning: Unknown GTE command 0x" << std::hex << (opcode & 0x3F) << std::dec << ", skipping\n";
        return;
    }
    it->second(opcode, cpu);
}

void GTEStub::register_command(int32_t opcode, CommandFunc func) {
    commands_[opcode] = func;
}

void GTEStub::unregister_command(int32_t opcode) {
    commands_.erase(opcode);
}

std::vector<int32_t> GTEStub::get_supported_commands() const {
    std::vector<int32_t> cmds;
    for (const auto& kv : commands_) {
        cmds.push_back(kv.first);
    }
    return cmds;
}

// Dummy command implementations - stubs that do nothing
// Real implementations would access CPUState to read inputs and write outputs
// The raw opcode is passed so implementations can decode fields in place

void cmd_rtps_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // RTPS: Round Perspective Transformation (single vertex)
    // Dummy: no operation, registers remain unchanged
}

void cmd_rtpt_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // RTPT: Round Perspective Transformation (triple vertex)
    // Dummy: no operation
}

void cmd_mvmva_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // MVMVA: Matrix Vector Multiply with Add
    // Dummy: no operation
}

void cmd_nclip_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // NCLIP: Normal Clipping
    // Dummy: no operation
}

void cmd_avsz3_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // AVSZ3: Average Z for 3 vertices
    // Dummy: no operation
}

void cmd_avsz4_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // AVSZ4: Average Z for 4 vertices
    // Dummy: no operation
}

void cmd_sqr_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // SQR: Square of Vector
    // Dummy: no operation
}

void cmd_op_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // OP: Outer Product
    // Dummy: no operation
}

void cmd_gpf_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // GPF: General Purpose Interpolation (flat shading)
    // Dummy: no operation
}

void cmd_gpl_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // GPL: General Purpose Interpolation with base (smooth shading)
    // Dummy: no operation
}

void cmd_ncs_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // NCS: Normal Color Single
    // Dummy: no operation
}

void cmd_nccs_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // NCCS: Normal Color Color Single
    // Dummy: no operation
}

void cmd_ncds_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // NCDS: Normal Color Depth Cue Single
    // Dummy: no operation
}

void cmd_dpcs_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // DPCS: Depth Cue Color Single
    // Dummy: no operation
}

void cmd_intpl_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // INTPL: Interpolation
    // Dummy: no operation
}

void cmd_dcpl_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // DCPL: Depth Cue Color Per light
    // Dummy: no operation
}

void cmd_dpct_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // DPCT: Depth Cueing (triple)
    // Dummy: no operation
}

void cmd_nct_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // NCT: Normal color (triple)
    // Dummy: no operation
}

void cmd_ncdt_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // NCDT: Normal color depth cue (triple vectors)
    // Dummy: no operation
}

void cmd_ncct_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // NCCT: Normal Color Color (triple vector)
    // Dummy: no operation
}

void cmd_cdp_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // CDP: Color Depth Que
    // Dummy: no operation
}

void cmd_cc_stub(int32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
    // CC: Color Color
    // Dummy: no operation
}

// Register all GTE commands with dummy implementations
// Opcodes are the real GTE command values (bits 5-0)
// The full opcode is assembled by the test framework from test data fields
void register_dummy_commands(GTEStub& stub) {
    stub.register_command(0x01, cmd_rtps_stub);
    stub.register_command(0x02, cmd_rtpt_stub);
    stub.register_command(0x06, cmd_nclip_stub);
    stub.register_command(0x0C, cmd_op_stub);
    stub.register_command(0x0E, cmd_ncds_stub);
    stub.register_command(0x10, cmd_dpcs_stub);
    stub.register_command(0x11, cmd_intpl_stub);
    stub.register_command(0x12, cmd_mvmva_stub);
    stub.register_command(0x1B, cmd_nccs_stub);
    stub.register_command(0x1E, cmd_ncs_stub);
    stub.register_command(0x28, cmd_sqr_stub);
    stub.register_command(0x29, cmd_dcpl_stub);
    stub.register_command(0x2D, cmd_avsz3_stub);
    stub.register_command(0x2E, cmd_avsz4_stub);
    stub.register_command(0x3D, cmd_gpf_stub);
    stub.register_command(0x3E, cmd_gpl_stub);
    stub.register_command(0x14, cmd_cdp_stub);
    stub.register_command(0x16, cmd_ncdt_stub);
    stub.register_command(0x20, cmd_nct_stub);
    stub.register_command(0x2A, cmd_dpct_stub);
    stub.register_command(0x3F, cmd_ncct_stub);
    stub.register_command(0x1C, cmd_cc_stub);
}

} // namespace gte
