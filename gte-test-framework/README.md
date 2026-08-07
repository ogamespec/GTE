# GTE Test Framework (GTE-Runner)

Console application for testing GTE (Geometry Transform Engine) implementations using JSON test datasets.

## Overview

GTE-Runner is a C++ testing framework that:
- Loads GTE test cases from JSON files
- Executes them against a GTEStub (portable GTE command interface)
- Compares actual register states with expected final states
- Reports pass/fail results with detailed mismatch information

## Project Structure

```
gte-test-framework/
├── Makefile
├── build.ps1
├── run.bat
├── README.md
├── include/
│   ├── json_parser.h      - Lightweight JSON parser (no dependencies)
│   ├── gte_model.h        - GTE register state model + CPUState interface
│   ├── dummy_api.h        - GTEStub interface + GTECPUState implementation
│   └── test_framework.h   - Test runner and reporter
└── src/
    ├── main.cpp           - Console application entry point
    ├── json_parser.cpp    - JSON parser implementation
    ├── gte_model.cpp      - Register state model + GTECPUState
    ├── dummy_api.cpp      - GTEStub implementation + dummy commands
    └── test_framework.cpp - Test loading and execution
```

## Building

### Requirements
- C++17 compatible compiler (g++, clang++, MSVC)
- Make (for Unix-like systems) or Visual Studio (for Windows)

### Unix/Linux/Mac

```bash
cd gte-test-framework
make
```

### Windows (MSVC)

```powershell
cd gte-test-framework
.\build.ps1
```

## Usage

```bash
# Run tests from parent directory v1/
./gte-runner ../v1

# Specify test directory
./gte-runner ./test_data

# Show only summary (no detailed report)
./gte-runner ../v1 -s

# Verbose output
./gte-runner ../v1 -v

# Show help
./gte-runner -h
```

### Command Line Options

| Option | Description |
|--------|-------------|
| `-h, --help` | Show help message |
| `-d, --dir <dir>` | Test directory path (default: ./v1) |
| `-s, --summary` | Show only summary, skip detailed report |
| `-v, --verbose` | Enable verbose output |

## GTEStub Interface

GTEStub is the portable interface for GTE implementation in PSX emulators.

### Design Principles

1. **Raw 32-bit opcode**: Accepts the raw GTE opcode (bits 24-0 of COP2 instruction)
2. **In-place field decoding**: Fields (sf, mx, v, cv, lm) are decoded from the opcode using pcsx-compatible bit masks
3. **CPUState interface**: Provides emulator-agnostic register access (CP2D/CP2C)
4. **Easy porting**: Implement CPUState and register command functions

### Porting to an Emulator

```cpp
// 1. Implement CPUState to wrap your emulator's CP2D/CP2C registers
class MyGTECPUState : public gte::CPUState {
public:
    int32_t get_data_d(int n) override {
        return psxRegs.CP2D.p[n].d;  // Your emulator's data register
    }
    void set_data_d(int n, int32_t value) override {
        psxRegs.CP2D.p[n].d = value;
    }
    int32_t get_control_d(int n) override {
        return psxRegs.CP2C.p[n].d;  // Your emulator's control register
    }
    void set_control_d(int n, int32_t value) override {
        psxRegs.CP2C.p[n].d = value;
    }
    int32_t get_data_raw(int n) override { return get_data_d(n); }
    void set_data_raw(int n, int32_t value) override { set_data_d(n, value); }
    int32_t get_control_raw(int n) override { return get_control_d(n); }
    void set_control_raw(int n, int32_t value) override { set_control_d(n, value); }
};

// 2. Create GTEStub and register command implementations
gte::GTEStub stub;

stub.register_command(0x01, [](int32_t opcode, gte::CPUState& cpu) {
    // RTPS implementation
    auto fields = gte::GTEFields::decode(opcode);
    int sf = fields.sf;  // Shift fraction flag
    // Access registers via cpu.get_data_d(), cpu.get_control_d(), etc.
    // ... implement RTPS logic ...
});

// 3. Execute GTE instructions
MyGTECPUState cpu_state;
// Set up initial state via cpu_state...
stub.execute_command(raw_gte_opcode, cpu_state);
```

### GTE Instruction Field Decoding (pcsx-compatible)

```cpp
// In pcsx: gteop = psxRegs.code & 0x1ffffff
// Fields are extracted from gteop using bit masks:

#define GTE_SF(op)   ((op >> 19) & 1)    // bit 19: shift fraction
#define GTE_MX(op)   ((op >> 17) & 3)    // bits 17-16: matrix select
#define GTE_V(op)    ((op >> 15) & 3)    // bits 15-14: vector select
#define GTE_CV(op)   ((op >> 13) & 3)    // bits 13-12: translation vector
#define GTE_LM(op)   ((op >> 10) & 1)    // bit 10: saturate mode
#define GTE_FUNCT(op) (op & 0x3F)         // bits 5-0: real GTE command
```

### Register Names (pcsx-compatible)

The framework provides register name constants matching pcsx naming:

```cpp
// Data registers (CP2D)
gte::RegNames::VXY0   // d0 - Vector 0 X/Y
gte::RegNames::VZ0    // d1 - Vector 0 Z
gte::RegNames::IR1    // d9 - Interpolate accumulator 1
gte::RegNames::MAC1   // d25 - Math accumulator 1
gte::RegNames::FLAG   // d31 - Flags register

// Control registers (CP2C)
gte::RegNames::RT11   // c0 - Rotation matrix element
gte::RegNames::TRX    // c9 - Translation X
gte::RegNames::RBK    // c21 - Background color R
gte::RegNames::RFC    // c32 - Far color R
gte::RegNames::H      // c37 - Projection distance
```

## Test File Format

Test files are JSON arrays of test cases. Each test case contains:

```json
{
    "name": "test description",
    "command": 1,
    "raw_opcode": 1073742209,
    "sf": 0,
    "mx": 0,
    "v": 0,
    "cv": 0,
    "lm": 0,
    "initial": {
        "d0": 0,
        "c0": 0
    },
    "final": {
        "d9": 0,
        "c31": 0
    }
}
```

### Opcode Fields

The GTE instruction is a 25-bit value (bits 24-0 of COP2 instruction):

| Bits | Field | Description |
|------|-------|-------------|
| 24-20 | fakeop | Fake GTE command number (emulator-specific) |
| 19 | sf | Shift fraction (1 = result >> 12) |
| 18-17 | mx | Matrix select (0=Rotation, 1=Light, 2=Color) |
| 16-15 | v | Vector select (0=V0, 1=V1, 2=V2, 3=IR) |
| 14-13 | cv | Translation vector (0=TR, 1=BK, 2=FC) |
| 12-11 | - | Reserved (zero) |
| 10 | lm | Saturate mode (0=signed, 1=unsigned) |
| 9-6 | - | Reserved (zero) |
| 5-0 | command | Real GTE command opcode |

When `raw_opcode` is present, fields are extracted from it using pcsx-compatible decoding.
When absent, the framework assembles the opcode from individual fields.

## Output Format

```
GTE Test Framework Runner
==========================

Test directory: ../v1
Registered commands: 22

Running tests...

Loading test file: ../v1/rtps.json
Loading test file: ../v1/rtpt.json
...

========================================
       GTE Test Framework Summary
========================================
Total tests:  119
Passed:       35
Failed:       84
Pass rate:    29.4%
========================================

========================================
       Detailed Test Report
========================================

--- ../v1/rtps.json ---
  [PASS] RTPS basic identity - all zeros (cmd: 0x01 op: 0x00000001)
  [FAIL] RTPS - simple translation (cmd: 0x01 op: 0x00000001)
         d9: expected 0x00000200 got 0x00000000
         d10: expected 0x00000400 got 0x00000000

========================================
```

## License

This framework is in the public domain (CC0), matching the GTE test suite license.
