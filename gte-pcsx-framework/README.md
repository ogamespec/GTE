# PCSX GTE Test Framework

C++ framework for testing the PCSX GTE (Geometry Transform Engine) implementation against the GTE test suite.

## Overview

This framework integrates the original PCSX GTE emulator code (`gte.c`) with the `gte-test-framework` to provide:

- Direct execution of PCSX's GTE instruction implementations
- JSON test case loading and validation
- Register state comparison against expected results
- Pass/fail reporting with detailed mismatch information

## Project Structure

```
gte-pcsx-framework/
├── Makefile              - Unix/Linux build script
├── build.bat             - Windows MSVC build script
├── README.md             - This file
├── include/              - Public headers
│   ├── pcsx_types.h      - Basic type definitions (u8, s32, u32, etc.)
│   ├── psxcommon.h       - GTE register types and macro accessors
│   ├── psxmem.h          - Memory access mocks
│   ├── r3000a.h          - R3000A CPU mock header
│   ├── gte_stub.h        - Base GTEStub interface (from gte-test-framework)
│   ├── gte_model.h       - GTE register state model (from gte-test-framework)
│   ├── json_parser.h     - JSON parser (from gte-test-framework)
│   ├── test_framework.h  - Test framework (from gte-test-framework)
│   ├── pcsx_gte_stub.h   - PCSX command registration
│   └── pcsx_gte_cpu_state.h - CPUState wrapper for RegisterState
├── src/                  - Source files
│   ├── gte.c             - Original PCSX GTE implementation (unchanged)
│   ├── gte.h             - Original PCSX GTE header (unchanged)
│   ├── pcsx_gte_stub.cpp - PCSX command registration
│   ├── pcsx_gte_cpu_state.cpp - CPUState/RegisterState bridge
│   └── main.cpp          - Entry point and test runner
└── pcsx-mocks/           - Additional mock files (if needed)
```

## Building

### Requirements

- C++17 compatible compiler (g++, clang++, MSVC)
- Make (for Unix-like systems) or Visual Studio (for Windows)

### Unix/Linux/Mac

```bash
cd gte-pcsx-framework
make
```

### Windows (MSVC)

```powershell
cd gte-pcsx-framework
.\build.bat
```

## Usage

```bash
# Run tests from default directory (./v1)
./gte-pcsx-runner

# Specify test directory
./gte-pcsx-runner -d ../v1

# Show help
./gte-pcsx-runner -h
```

## How It Works

The framework bridges the PCSX GTE implementation with the test framework:

1. **Test Loading**: JSON test files are parsed to extract initial and expected final register states
2. **State Sync**: Initial state is copied to the global `psxRegs_global` structure that PCSX's `gte.c` expects
3. **Execution**: Each GTE command is executed by calling the corresponding PCSX function (e.g., `gteRTPS()`, `gteMVMVA()`)
4. **Result Extraction**: After execution, the final register state is read back from `psxRegs_global`
5. **Comparison**: The actual state is compared against the expected state from the test file

### Key Components

- **PCSXGTEStub**: Registers all 22 PCSX GTE commands as GTEStub handlers
- **PCSXGTECPUState**: Wraps `RegisterState` and provides sync methods to/from `psxRegs_global`
- **gte.c**: Original PCSX GTE implementation, kept unchanged as specified

### Integration with gte-test-framework

This framework extends the base `gte-test-framework` by:

- Using the same `RegisterState` model for test data
- Implementing the `CPUState` interface to bridge to PCSX globals
- Reusing the JSON test file format from `v1/`
- Providing a direct path to execute the PCSX implementation

## Test Data

Test files are located in the `v1/` directory (parent directory). They cover all 22 GTE instructions:

| Test File | GTE Command | Description |
|-----------|-------------|-------------|
| rtps.json | RTPS | Rear Transform Perspective Single |
| rtpt.json | RTPT | Rear Transform Perspective Triple |
| mvmva.json | MVMVA | Matrix Vector Multiply Add |
| nclip.json | NCLIP | Normal Clip |
| op.json | OP | Outer Product |
| dpcs.json | DPCS | Depth Cue Single |
| intpl.json | INTPL | Interpolate |
| cdp.json | CDP | Color Depth Queue |
| ncdt.json | NCDT | Normal Color Depth Cue Triple |
| nccs.json | NCCS | Normal Color Color Single |
| cc.json | CC | Color Color |
| ncs.json | NCS | Normal Color Single |
| nct.json | NCT | Normal Color Triple |
| sqr.json | SQR | Square |
| dcpl.json | DCPL | Depth Cue Color light |
| dpct.json | DPCT | Depth Cueing Triple |
| avsz3.json | AVSZ3 | Average Z Single |
| avsz4.json | AVSZ4 | Average Z Triple |
| gpf.json | GPF | General Purpose Filter |
| gpl.json | GPL | General Purpose Lift |
| ncct.json | NCCT | Normal Color Color Triple |

## License

The PCSX GTE implementation (`gte.c`) is licensed under the GNU General Public License v2 or later (Copyright 2003-2013 smf / PCSX-df Team / PCSX team).

The framework code (excluding `gte.c`) is provided under CC0 (public domain), matching the GTE test suite license.
