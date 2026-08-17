#include "gte_stub.h"
#include <iostream>
#include <sstream>

namespace gte {

// GTE opcode assembly according to GTE specification (gte.txt)
//
// Bit layout:
//   31-25: COP2 imm25 opcode (0100101b) - always zero in our context
//   24-20: Fake GTE command number
//   19:    sf (shift fraction)
//   18-17: mx (matrix select)
//   16-15: v (vector select)
//   14-13: cv (translation vector select)
//   12-11: always zero
//   10:    lm (saturate mode)
//   9-6:   always zero
//   5-0:   command (real GTE command)
uint32_t assemble_gte_opcode(int32_t command, int32_t fakeop,
                              int32_t sf, int32_t mx, int32_t v,
                              int32_t cv, int32_t lm) {
    return static_cast<uint32_t>(
        command |
        (fakeop << 20) |
        (sf << 19) |
        (mx << 17) |
        (v << 15) |
        (cv << 13) |
        (lm << 10)
    );
}

// GTEStub implementation
GTEStub::GTEStub() {}

GTEStub::~GTEStub() {}

GTEFields GTEStub::decode_fields(uint32_t opcode) const {
    return GTEFields::from_opcode(opcode);
}

void GTEStub::gte_execute(uint32_t opcode, CPUState& cpu) {
    int32_t cmd = static_cast<int32_t>(opcode & 0x3F);
    auto it = commands_.find(cmd);
    if (it == commands_.end()) {
        std::cerr << "Warning: Unknown GTE command 0x" << std::hex << (cmd & 0x3F) << std::dec << ", skipping\n";
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

// Dummy command implementations - no-op stubs
// Real implementations would access CPUState registers by index (0-31)
// and decode GTEFields from the opcode to get sf, mx, v, cv, lm parameters

void cmd_rtps_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_rtpt_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_mvmva_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_nclip_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_avsz3_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_avsz4_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_sqr_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_op_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_gpf_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_gpl_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_ncs_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_nccs_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_ncds_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_dpcs_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_intpl_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_dcpl_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_dpct_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_nct_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_ncdt_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_ncct_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_cdp_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

void cmd_cc_stub(uint32_t opcode, CPUState& cpu) {
    (void)opcode; (void)cpu;
}

// Register all GTE commands with dummy implementations
void register_dummy_commands(GTEStub& stub) {
    stub.register_command(0x01, cmd_rtps_stub);
    stub.register_command(0x30, cmd_rtpt_stub);
    stub.register_command(0x06, cmd_nclip_stub);
    stub.register_command(0x0C, cmd_op_stub);
    stub.register_command(0x13, cmd_ncds_stub);
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

}
