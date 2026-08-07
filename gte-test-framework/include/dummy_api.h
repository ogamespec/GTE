#ifndef GTE_DUMMY_API_H
#define GTE_DUMMY_API_H

#include "gte_model.h"
#include <string>
#include <functional>
#include <map>
#include <vector>

namespace gte {

class DummyGTEAPI {
public:
    using CommandFunc = std::function<void(RegisterState&)>;

    DummyGTEAPI();

    void execute_command(const std::string& command, RegisterState& state,
                         int32_t sf = 0, int32_t mx = 0, int32_t v = 0,
                         int32_t cv = 0, int32_t lm = 0);

    void register_command(const std::string& opcode, CommandFunc func);
    void unregister_command(const std::string& opcode);

    std::vector<std::string> get_supported_commands() const;

private:
    std::map<std::string, CommandFunc> commands_;

    void cmd_rtps(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_rtpt(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_mvmva(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_nclip(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_avsz3(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_avsz4(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_sqr(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_op(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_gpf(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_gpl(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_ncs(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_nccs(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_ncds(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_dpcs(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_intpl(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
    void cmd_dcpl(RegisterState& state, int32_t sf, int32_t mx, int32_t v, int32_t cv, int32_t lm);
};

}

#endif
