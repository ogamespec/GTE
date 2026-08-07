#include "gte_model.h"
#include <cstring>
#include <climits>
#include <cstdlib>

namespace gte {

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

// Static helper methods
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
    // 32-bit saturation is already handled by int32_t range
    return value;
}

int32_t GTEModel::shift_result(int32_t value, int32_t sf) {
    if (sf) {
        // Arithmetic right shift by 12 bits
        return value >> 12;
    }
    return value;
}

RegisterState GTEModel::get_state() const {
    // This is a dummy class, state is managed externally
    return RegisterState();
}

void GTEModel::set_state(const RegisterState& state) {
    // This is a dummy class, state is managed externally
}

}
