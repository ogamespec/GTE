#ifndef GTE_PCSX_GTE_STUB_H
#define GTE_PCSX_GTE_STUB_H

#include "gte_stub.h"
#include <cstdint>
#include <functional>
#include <map>
#include <vector>

namespace gte {

class PCSXGTEStub : public GTEStub {
public:
    PCSXGTEStub();
    ~PCSXGTEStub() override;

    // Register all PCSX GTE command implementations
    void register_pcsx_commands();

    // Get the number of registered PCSX commands
    int get_pcsx_command_count() const;

private:
    int pcsx_command_count_;
};

}

#endif
