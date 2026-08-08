#include "pcsx_gte_cpu_state.h"
#include "psxcommon.h"

namespace gte {

PCSXGTECPUState::PCSXGTECPUState() {}

PCSXGTECPUState::PCSXGTECPUState(const RegisterState& state) : state_(state) {}

void PCSXGTECPUState::set_state(const RegisterState& state) {
    state_ = state;
}

RegisterState PCSXGTECPUState::get_state() const {
    return state_;
}

int32_t PCSXGTECPUState::get_data_d(int n) const {
    if (n < 0 || n > 31) return 0;
    return state_.data[n];
}

void PCSXGTECPUState::set_data_d(int n, int32_t value) {
    if (n < 0 || n > 31) return;
    state_.data[n] = value;
}

int32_t PCSXGTECPUState::get_control_d(int n) const {
    if (n < 0 || n > 31) return 0;
    return state_.control[n];
}

void PCSXGTECPUState::set_control_d(int n, int32_t value) {
    if (n < 0 || n > 31) return;
    state_.control[n] = value;
}

int32_t PCSXGTECPUState::get_data_raw(int n) const {
    return get_data_d(n);
}

void PCSXGTECPUState::set_data_raw(int n, int32_t value) {
    set_data_d(n, value);
}

int32_t PCSXGTECPUState::get_control_raw(int n) const {
    return get_control_d(n);
}

void PCSXGTECPUState::set_control_raw(int n, int32_t value) {
    set_control_d(n, value);
}

void PCSXGTECPUState::sync_to_psx_regs() const {
    for (int i = 0; i < 32; i++) {
        psxRegs_global.CP2D.p[i].d = state_.data[i];
        psxRegs_global.CP2C.p[i].d = state_.control[i];
    }
}

void PCSXGTECPUState::sync_from_psx_regs() {
    for (int i = 0; i < 32; i++) {
        state_.data[i] = psxRegs_global.CP2D.p[i].d;
        state_.control[i] = psxRegs_global.CP2C.p[i].d;
    }
}

}
