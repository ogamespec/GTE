#ifndef GTE_PCSX_GTE_STUB_H
#define GTE_PCSX_GTE_STUB_H

#include <cstdint>
#include <functional>
#include <map>
#include <vector>

namespace gte {

class PCSXGTEStub {
public:
    using CommandFunc = std::function<void(uint32_t opcode, void* cpu)>;

    PCSXGTEStub();
    ~PCSXGTEStub();

    // Execute a GTE command with the given opcode
    void gte_execute(uint32_t opcode, void* cpu);

    // Register/unregister command implementations
    void register_command(int32_t opcode, CommandFunc func);
    void unregister_command(int32_t opcode);

    std::vector<int32_t> get_supported_commands() const;

    // Register all PCSX GTE command implementations
    void register_pcsx_commands();

    // Get the number of registered PCSX commands
    int get_pcsx_command_count() const;

private:
    std::map<int32_t, CommandFunc> commands_;
    int pcsx_command_count_;
};

}

#endif
