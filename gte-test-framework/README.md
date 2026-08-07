# GTE Test Framework (GTE-Runner)

Console application for testing GTE (Geometry Transform Engine) implementations using JSON test datasets.

## Overview

GTE-Runner is a C++ testing framework that:
- Loads GTE test cases from JSON files
- Executes them against a Dummy GTE API (stub implementations)
- Compares actual register states with expected final states
- Reports pass/fail results with detailed mismatch information

## Project Structure

```
gte-test-framework/
├── Makefile
├── build.ps1
├── build.bat
├── run.bat
├── README.md
├── include/
│   ├── json_parser.h      - Lightweight JSON parser (no dependencies)
│   ├── gte_model.h        - GTE register state model
│   ├── dummy_api.h        - Dummy GTE API interface
│   └── test_framework.h   - Test runner and reporter
└── src/
    ├── main.cpp           - Console application entry point
    ├── json_parser.cpp    - JSON parser implementation
    ├── gte_model.cpp      - Register state model
    ├── dummy_api.cpp      - Dummy GTE command stubs
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

## Dummy GTE API

The framework includes a Dummy GTE API with **stub implementations** for all 16 GTE commands.

These stubs are designed to be replaced by real GTE instruction implementations from emulators.

| Opcode | Command | Description |
|--------|---------|-------------|
| 0x01 | RTPS | Round Perspective Transformation (single) |
| 0x02 | RTPT | Round Perspective Transformation (triple) |
| 0x06 | NCLIP | Normal Clipping |
| 0x0C | OP | Outer Product |
| 0x0E | NCDS | Normal Color Depth Cue Single |
| 0x10 | DPCS | Depth Cue Color Single |
| 0x11 | INTPL | Interpolation |
| 0x12 | MVMVA | Matrix Vector Multiply with Add |
| 0x1B | NCCS | Normal Color Color Single |
| 0x1E | NCS | Normal Color Single |
| 0x28 | SQR | Square of Vector |
| 0x29 | DCPL | Depth Cue Color Per light |
| 0x2D | AVSZ3 | Average Z for 3 vertices |
| 0x2E | AVSZ4 | Average Z for 4 vertices |
| 0x3D | GPF | General Purpose Interpolation (flat) |
| 0x3E | GPL | General Purpose Interpolation (smooth) |

### Using Real Implementations

Replace the dummy command implementations with real GTE instruction logic:

```cpp
// In dummy_api.cpp, replace the stub:
void DummyGTEAPI::cmd_rtps(RegisterState& state, ...) {
    // Implement actual RTPS instruction
    // Read input registers, compute result, write output registers
}
```

Or register custom implementations at runtime:

```cpp
DummyGTEAPI api;
api.register_command("01", [](RegisterState& s) {
    // Custom RTPS implementation
});
```

## Test File Format

Test files are JSON arrays of test cases. Each test case contains:

```json
{
    "name": "test description",
    "command": "01",
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

See the main GTE repository README for full test format documentation.

## Output Format

```
GTE Test Framework Runner
==========================

Test directory: ../v1
Registered commands: 16

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
  [PASS] RTPS basic identity - all zeros (cmd: 0x01)
  [FAIL] RTPS - simple translation (cmd: 0x01)
         d9: expected 0x00000200 got 0x00000000
         d10: expected 0x00000400 got 0x00000000

========================================
```

## License

This framework is in the public domain (CC0), matching the GTE test suite license.
