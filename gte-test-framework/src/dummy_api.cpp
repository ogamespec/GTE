#include "dummy_api.h"
#include <iostream>
#include <sstream>

namespace gte {

DummyGTEAPI::DummyGTEAPI() {
    // Register all GTE commands with dummy (pass-through) implementations
    register_command(0x01, [this](RegisterState& s) { cmd_rtps(s, 0, 0, 0, 0, 0); });
    register_command(0x02, [this](RegisterState& s) { cmd_rtpt(s, 0, 0, 0, 0, 0); });
    register_command(0x06, [this](RegisterState& s) { cmd_nclip(s, 0, 0, 0, 0, 0); });
    register_command(0x0C, [this](RegisterState& s) { cmd_op(s, 0, 0, 0, 0, 0); });
    register_command(0x0E, [this](RegisterState& s) { cmd_ncds(s, 0, 0, 0, 0, 0); });
    register_command(0x10, [this](RegisterState& s) { cmd_dpcs(s, 0, 0, 0, 0, 0); });
    register_command(0x11, [this](RegisterState& s) { cmd_intpl(s, 0, 0, 0, 0, 0); });
    register_command(0x12, [this](RegisterState& s) { cmd_mvmva(s, 0, 0, 0, 0, 0); });
    register_command(0x1B, [this](RegisterState& s) { cmd_nccs(s, 0, 0, 0, 0, 0); });
    register_command(0x1E, [this](RegisterState& s) { cmd_ncs(s, 0, 0, 0, 0, 0); });
    register_command(0x28, [this](RegisterState& s) { cmd_sqr(s, 0, 0, 0, 0, 0); });
    register_command(0x29, [this](RegisterState& s) { cmd_dcpl(s, 0, 0, 0, 0, 0); });
    register_command(0x2D, [this](RegisterState& s) { cmd_avsz3(s, 0, 0, 0, 0, 0); });
    register_command(0x2E, [this](RegisterState& s) { cmd_avsz4(s, 0, 0, 0, 0, 0); });
    register_command(0x3D, [this](RegisterState& s) { cmd_gpf(s, 0, 0, 0, 0, 0); });
    register_command(0x3E, [this](RegisterState& s) { cmd_gpl(s, 0, 0, 0, 0, 0); });
}

void DummyGTEAPI::execute_command(int32_t command, RegisterState& state,
                                   int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    auto it = commands_.find(command);
    if (it == commands_.end()) {
        std::cerr << "Warning: Unknown GTE command 0x" << std::hex << command << std::dec << ", skipping\n";
        return;
    }
    it->second(state);
}

void DummyGTEAPI::register_command(int32_t opcode, CommandFunc func) {
    commands_[opcode] = func;
}

void DummyGTEAPI::unregister_command(int32_t opcode) {
    commands_.erase(opcode);
}

std::vector<int32_t> DummyGTEAPI::get_supported_commands() const {
    std::vector<int32_t> cmds;
    for (const auto& kv : commands_) {
        cmds.push_back(kv.first);
    }
    return cmds;
}

// Dummy implementations - these are stubs that do nothing or pass through values.
// Real implementations would contain the actual GTE instruction logic.
// These dummies are designed to be replaced by real emulator implementations.

void DummyGTEAPI::cmd_rtps(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // RTPS: Round Perspective Transformation (single vertex)
    // Dummy: no operation, registers remain unchanged
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_rtpt(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // RTPT: Round Perspective Transformation (triple vertex)
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_mvmva(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // MVMVA: Matrix Vector Multiply with Add
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_nclip(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // NCLIP: Normal Clipping
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_avsz3(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // AVSZ3: Average Z for 3 vertices
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_avsz4(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // AVSZ4: Average Z for 4 vertices
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_sqr(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // SQR: Square of Vector
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_op(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // OP: Outer Product
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_gpf(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // GPF: General Purpose Interpolation (flat shading)
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_gpl(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // GPL: General Purpose Interpolation with base (smooth shading)
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_ncs(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // NCS: Normal Color Single
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_nccs(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // NCCS: Normal Color Color Single
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_ncds(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // NCDS: Normal Color Depth Cue Single
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_dpcs(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // DPCS: Depth Cue Color Single
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_intpl(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // INTPL: Interpolation
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

void DummyGTEAPI::cmd_dcpl(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm) {
    // DCPL: Depth Cue Color Per light
    // Dummy: no operation
    (void)state; (void)sf; (void)mx; (void)v; (void)cv; (void)lm;
}

}
