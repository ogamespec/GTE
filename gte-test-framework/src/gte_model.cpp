#include "gte_model.h"
#include "dummy_api.h"
#include <cstring>
#include <climits>
#include <cstdlib>

namespace gte {

// RegisterState implementation
RegisterState::RegisterState() {
    clear();
}

void RegisterState::clear() {
    std::memset(data, 0, sizeof(data));
    std::memset(control, 0, sizeof(control));
}

bool RegisterState::operator==(const RegisterState& other) const {
    return std::memcmp(data, other.data, sizeof(data)) == 0 &&
           std::memcmp(control, other.control, sizeof(control)) == 0;
}

bool RegisterState::operator!=(const RegisterState& other) const {
    return !(*this == other);
}

int32_t RegisterState::get_data(int index) const {
    if (index < 0 || index > 31) {
        return 0;
    }
    return data[index];
}

int32_t RegisterState::get_control(int index) const {
    if (index < 0 || index > 31) {
        return 0;
    }
    return control[index];
}

void RegisterState::set_data(int index, int32_t value) {
    if (index >= 0 && index <= 31) {
        data[index] = value;
    }
}

void RegisterState::set_control(int index, int32_t value) {
    if (index >= 0 && index <= 31) {
        control[index] = value;
    }
}

void RegisterState::apply_to_cpu(CPUState& cpu) const {
    for (int i = 0; i < 32; i++) {
        cpu.set_data_d(i, data[i]);
        cpu.set_control_d(i, control[i]);
    }
}

void RegisterState::load_from_cpu(const CPUState& cpu) {
    for (int i = 0; i < 32; i++) {
        data[i] = cpu.get_data_d(i);
        control[i] = cpu.get_control_d(i);
    }
}

// GTEModel static helpers
int32_t GTEModel::saturate_16_signed(int32_t value) {
    if (value > 0x7FFF) return 0x7FFF;
    if (value < -0x8000) return -0x8000;
    return value;
}

int32_t GTEModel::saturate_16_unsigned(int32_t value) {
    if (value > 0xFFFF) return 0xFFFF;
    if (value < 0) return 0;
    return value;
}

int32_t GTEModel::saturate_32(int32_t value) {
    return value;
}

int32_t GTEModel::shift_result(int32_t value, int32_t sf) {
    if (sf) {
        return value >> 12;
    }
    return value;
}

RegisterState GTEModel::get_state() const {
    return RegisterState();
}

void GTEModel::set_state(const RegisterState& state) {
    (void)state;
}

// GTECPUState implementation
GTECPUState::GTECPUState() {}

GTECPUState::GTECPUState(const RegisterState& state) : state_(state) {}

void GTECPUState::set_state(const RegisterState& state) const {
    state_ = state;
}

RegisterState GTECPUState::get_state() const {
    return state_;
}

int32_t GTECPUState::get_data_d(int n) const {
    if (n < 0 || n > 31) return 0;
    return state_.data[n];
}

void GTECPUState::set_data_d(int n, int32_t value) {
    if (n >= 0 && n <= 31) {
        state_.data[n] = value;
    }
}

int32_t GTECPUState::get_control_d(int n) const {
    if (n < 0 || n > 31) return 0;
    return state_.control[n];
}

void GTECPUState::set_control_d(int n, int32_t value) {
    if (n >= 0 && n <= 31) {
        state_.control[n] = value;
    }
}

int32_t GTECPUState::get_data_raw(int n) const {
    return get_data_d(n);
}

void GTECPUState::set_data_raw(int n, int32_t value) {
    set_data_d(n, value);
}

int32_t GTECPUState::get_control_raw(int n) const {
    return get_control_d(n);
}

void GTECPUState::set_control_raw(int n, int32_t value) {
    set_control_d(n, value);
}

}
