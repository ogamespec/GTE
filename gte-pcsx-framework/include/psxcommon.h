#ifndef __PCSX_REGS_H__
#define __PCSX_REGS_H__

#include "pcsx_types.h"

// Register union types matching pcsx layout
typedef struct {
    struct {
        s32 l;
        s32 h;
    } sw;
    union {
        struct {
            s32 l;
            s32 h;
        } w;
        struct {
            s32 l;
            s32 h;
            s32 h2;
            s32 h3;
        } b;
        s32 d;
    };
} PSXRegPair;

typedef struct {
    union {
        s32 d;
        struct {
            s32 l;
            s32 h;
        } sw;
        struct {
            s32 l;
            s32 h;
        } w;
        struct {
            s32 l;
            s32 h;
            s32 h2;
            s32 h3;
        } b;
        s64 sd;
    };
} PSXReg;

// GTE data registers (CP2D) - 32 registers
// GTE control registers (CP2C) - 32 registers
// Each register is a union type matching PS1 hardware behavior

typedef struct {
    PSXReg p[32];
} CP2DReg;

typedef struct {
    PSXReg p[32];
} CP2CReg;

// R3000A CPU registers
typedef struct {
    struct {
        u32 code;
        u32 cycle;
        u32 type;
        u32 reg;
        u32 rd;
        u32 imm;
    } CP2;
    struct {
        u32 n;
        u32 s;
        u32 t;
        u32 d;
        u32 rt;
    } instr;
} PSXRec;

// GPR (general purpose registers)
typedef struct {
    u32 _r[32];
} GPRReg;

// Config
typedef struct {
    int Widescreen;
} ConfigType;

// GPU callback
typedef void (*GPUAddVertexFunc)(s32 sx, s32 sy, s32 ix, s32 iy, s32 iz);

// Global state (mocked)
extern CP2DReg g_CP2D;
extern CP2CReg g_CP2C;
extern GPRReg  g_GPR;
extern ConfigType g_Config;
extern u32 g_psxRegs_code;
extern PSXRec g_psxRec;

// Macro-compatible accessors
#define psxRegs g_psxRegs_global

typedef struct {
    CP2DReg CP2D;
    CP2CReg CP2C;
    GPRReg  GPR;
    struct {
        u32 code;
    } temp;
} PSXRegsGlobal;

extern PSXRegsGlobal psxRegs_global;

#define Config g_Config

#define _Rd_  g_psxRec.instr.d
#define _Rs_  g_psxRec.instr.s
#define _Rt_  g_psxRec.instr.rt
#define _Imm_ g_psxRec.instr.imm
#define _oB_  (g_psxRegs_global.GPR._r[_Rs_] + _Imm_)

// GPU callback - will be set by the test framework
extern GPUAddVertexFunc GPU_addVertex;

// Config
extern int Widescreen;

#endif
