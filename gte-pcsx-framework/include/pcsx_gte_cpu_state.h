#ifndef GTE_PCSX_GTE_CPU_STATE_H
#define GTE_PCSX_GTE_CPU_STATE_H

#include "gte_model.h"
#include "gte_stub.h"

namespace gte {

class PCSXGTECPUState : public CPUState {
public:
    PCSXGTECPUState();
    explicit PCSXGTECPUState(const RegisterState& state);

    void set_state(const RegisterState& state);
    RegisterState get_state() const;

    int32_t get_data_d(int n) const override;
    void set_data_d(int n, int32_t value) override;
    int32_t get_control_d(int n) const override;
    void set_control_d(int n, int32_t value) override;
    int32_t get_data_raw(int n) const override;
    void set_data_raw(int n, int32_t value) override;
    int32_t get_control_raw(int n) const override;
    void set_control_raw(int n, int32_t value) override;

    void sync_to_psx_regs() const;
    void sync_from_psx_regs();

private:
    mutable RegisterState state_;
};

}

#endif
