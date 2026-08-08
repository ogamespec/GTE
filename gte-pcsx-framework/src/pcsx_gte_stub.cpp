#include "pcsx_gte_stub.h"
#include "gte.h"

namespace gte {

PCSXGTEStub::PCSXGTEStub() : pcsx_command_count_(0) {}

PCSXGTEStub::~PCSXGTEStub() {}

void PCSXGTEStub::gte_execute(uint32_t opcode, void* cpu) {
    (void)cpu;
    int32_t cmd = static_cast<int32_t>(opcode & 0x3F);
    auto it = commands_.find(cmd);
    if (it == commands_.end()) {
        return;
    }
    it->second(opcode, cpu);
}

void PCSXGTEStub::register_command(int32_t opcode, CommandFunc func) {
    commands_[opcode] = func;
}

void PCSXGTEStub::unregister_command(int32_t opcode) {
    commands_.erase(opcode);
}

std::vector<int32_t> PCSXGTEStub::get_supported_commands() const {
    std::vector<int32_t> cmds;
    for (const auto& kv : commands_) {
        cmds.push_back(kv.first);
    }
    return cmds;
}

void PCSXGTEStub::register_pcsx_commands() {
    register_command(0x01, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteRTPS();
    });
    register_command(0x02, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteRTPT();
    });
    register_command(0x06, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteNCLIP();
    });
    register_command(0x0C, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteOP();
    });
    register_command(0x0E, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteNCDS();
    });
    register_command(0x10, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteDPCS();
    });
    register_command(0x11, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteINTPL();
    });
    register_command(0x12, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteMVMVA();
    });
    register_command(0x1B, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteNCCS();
    });
    register_command(0x1E, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteNCS();
    });
    register_command(0x28, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteSQR();
    });
    register_command(0x29, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteDCPL();
    });
    register_command(0x2D, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteAVSZ3();
    });
    register_command(0x2E, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteAVSZ4();
    });
    register_command(0x3D, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteGPF();
    });
    register_command(0x3E, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteGPL();
    });
    register_command(0x14, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteCDP();
    });
    register_command(0x16, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteNCDT();
    });
    register_command(0x20, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteNCT();
    });
    register_command(0x2A, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteDPCT();
    });
    register_command(0x3F, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteNCCT();
    });
    register_command(0x1C, [](uint32_t opcode, void* cpu) {
        (void)opcode; (void)cpu;
        gteCC();
    });

    pcsx_command_count_ = 22;
}

int PCSXGTEStub::get_pcsx_command_count() const {
    return pcsx_command_count_;
}

}
