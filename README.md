# GTE (Geometry Transform Engine) Test Suite

Repository for GTE JSON test files for the PlayStation Geometry Transform Engine.

## Overview

The GTE is a coprocessor in the PlayStation (PS1) responsible for geometric and color calculations using fixed-point arithmetic. This repository contains comprehensive test datasets for verifying GTE instruction implementations.

These GTE tests focus on register state before and after executing GTE commands, covering all edge cases and saturation conditions.

## Test Format

Each `.json` file contains an array of test cases. Each test case follows this structure:

```json
{
    "name": "test description",
    "command": 1,
    "initial": {
        "d0": 0,
        "d1": 0,
        ...
        "d31": 0,
        "c0": 0,
        "c1": 0,
        ...
        "c31": 0
    },
    "final": {
        "d0": 0,
        "d1": 0,
        ...
        "d31": 0,
        "c0": 0,
        "c1": 0,
        ...
        "c31": 0
    }
}
```

### Register naming convention

| Register | GTE Register | Description |
|----------|-------------|-------------|
| `d0-d1` | VXY0, VZ0 | Vector 0 (X,Y,Z) |
| `d2-d3` | VXY1, VZ1 | Vector 1 (X,Y,Z) |
| `d4-d5` | VXY2, VZ2 | Vector 2 (X,Y,Z) |
| `d6` | RGBC | Color/code value |
| `d7` | OTZ | Average Z value |
| `d8` | IR0 | 16-bit Accumulator (Interpolate) |
| `d9-d11` | IR1, IR2, IR3 | 16-bit Accumulator (Vector) |
| `d12-d15` | SXY0-SXY2, SXYP | Screen XY-coordinate FIFO |
| `d16-d19` | SZ0-SZ3 | Screen Z-coordinate FIFO |
| `d20-d22` | RGB0-RGB2 | Color CRGB-code/color FIFO |
| `d24` | MAC0 | 32-bit Maths Accumulator |
| `d25-d27` | MAC1-MAC3 | 32-bit Maths Accumulators (Vector) |
| `d28` | IRGB | Convert RGB Color Input |
| `d29` | ORGB | Convert RGB Color Output |
| `d30` | LZCS | Count Leading-Zeroes Source |
| `d31` | LZCR | Count Leading-Zeroes Result |
| `c0-c8` | RT11-RT33 | Rotation matrix |
| `c9-c11` | TRX, TRY, TRZ | Translation vector |
| `c12-c14` | L11-L33 | Light matrix |
| `c15-c17` | RBK, GBK, BBK | Background color |
| `c18-c20` | LR1-LB3 | Light color matrix |
| `c21-c23` | RFC, GFC, BFC | Far color |
| `c24-c25` | OFX, OFY | Screen offset |
| `c26` | H | Projection plane distance |
| `c27` | DQA | Depth queuing parameter A |
| `c28` | DQB | Depth queuing parameter B |
| `c29-c30` | ZSF3, ZSF4 | Z scale factors |
| `c31` | FLAG | Calculation error flags |

Values are decimal integers. Registers not relevant to a specific instruction may be omitted from `initial`/`final` (treated as 0).

## Test Files

| File | Instruction | Description |
|------|-------------|-------------|
| `rtps.json` | RTPS | Perspective Transformation (single) |
| `rtpt.json` | RTPT | Perspective Transformation (triple) |
| `mvmva.json` | MVMVA | Multiply Matrix add Vector |
| `nclip.json` | NCLIP | Normal clipping |
| `avsz3.json` | AVSZ3 | Average of three Z values |
| `avsz4.json` | AVSZ4 | Average of four Z values |
| `sqr.json` | SQR | Square vector |
| `op.json` | OP | Outer product of two vectors |
| `gpf.json` | GPF | General purpose interpolation |
| `gpl.json` | GPL | General purpose interpolation with base |
| `ncs.json` | NCS | Normal color (single) |
| `nccs.json` | NCCS | Normal Color Color (single vector) |
| `ncds.json` | NCDS | Normal color depth cue (single vector) |
| `dpcs.json` | DPCS | Depth Cueing (single) |
| `intpl.json` | INTPL | Interpolation |
| `dcpl.json` | DCPL | Depth Cue Color light |
| `dpct.json` | DPCT | Depth Cueing (triple) |
| `nct.json` | NCT | Normal color (triple) |
| `ncdt.json` | NCDT | Normal color depth cue (triple vectors) |
| `ncct.json` | NCCT | Normal Color Color (triple vector) |
| `cdp.json` | CDP | Color Depth Queue |
| `cc.json` | CC | Color Color |

## Additional Test Parameters

Each test case can include optional parameters to control test behavior. The default value for all parameters is `0` if omitted.

| Parameter | Type | Description |
|-----------|------|-------------|
| `sf` | 0 or 1 | Shift flag. When `sf=1`, the result is shifted right by 12 bits (12-bit fraction shift). Default: `0` |
| `mx` | 0 or 1 | Matrix select. Selects which rotation matrix to use (RT11-RT33 or L11-L33). Default: `0` |
| `v` | 0 or 1 | Vector select. Selects which input vector to use (VXY0/VZ0, VXY1/VZ1, or VXY2/VZ2). Default: `0` |
| `cv` | 0 or 1 | Color select. Selects which color source to use (LR1-LB3 or RBK/GBK/BBK). Default: `0` |
| `lm` | 0 or 1 | Light mode. When `lm=0`, uses signed saturation (0x8000/0x7FFF). When `lm=1`, uses unsigned saturation (0x0000/0xFFFF). Default: `0` |

Example:

```json
{
    "name": "test with sf=1",
    "command": 1,
    "sf": 1,
    "initial": { ... },
    "final": { ... }
}
```

## GTE Test Framework

The `gte-test-framework/` directory contains a C++ console application for running these tests against GTE implementations.

### Building

```bash
cd gte-test-framework
make        # Unix/Linux/Mac
.\build.ps1 # Windows (MSVC)
```

### Running

```bash
./gte-runner ../v1       # Run all tests
./gte-runner ../v1 -s    # Summary only
./gte-runner ../v1 -v    # Verbose output
```

See `gte-test-framework/README.md` for detailed documentation on:
- GTEStub interface for emulator porting
- CPUState interface for register access
- GTE instruction field decoding (pcsx-compatible)
- Register name constants

## Edge Cases Covered

Each instruction test file covers:

- **Zero values** - all-zero inputs
- **Maximum positive values** - 0x7FFF (16-bit) / 0x7FFFFFFF (32-bit)
- **Maximum negative values** - 0x8000 (16-bit) / 0x80000000 (32-bit)
- **Boundary values** - just inside/outside saturation thresholds
- **Division edge cases** - near-zero divisors, overflow conditions
- **Overflow/saturation** - values that trigger FLAG bits
- **Special matrix modes** - MVMVA with different matrix/vector/translation selections
- **Both sf modes** - `sf=0` (no shift) and `sf=1` (12-bit fraction shift)
- **Both lm modes** - `lm=0` (signed saturation) and `lm=1` (unsigned saturation)

## Usage

```python
import json

with open('v1/rtps.json') as f:
    tests = json.load(f)

for test in tests:
    # Assemble GTE opcode from test properties
    raw_opcode = (
        ((test.get('fakeop', 0)) & 0x1F) << 20 |
        ((test.get('sf', 0)) & 1) << 19 |
        ((test.get('mx', 0)) & 3) << 17 |
        ((test.get('v', 0)) & 3) << 15 |
        ((test.get('cv', 0)) & 3) << 13 |
        ((test.get('lm', 0)) & 1) << 10 |
        (test['command'] & 0x3F)
    )

    # Set GTE registers to initial state
    set_gte_state(test['initial'])

    # Execute the GTE instruction
    execute_gte_opcode(raw_opcode)

    # Compare current GTE state to expected final state
    assert get_gte_state() == test['final']
```

## References

- PlayStation GTE Technical Reference: `gte.txt`
- Similar Z80 test suite: https://github.com/SingleStepTests/z80


:warning: Content is mostly AI generated.

![x83ag3e0ug3f1.png](x83ag3e0ug3f1.png)
