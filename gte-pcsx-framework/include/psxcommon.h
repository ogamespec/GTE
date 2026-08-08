#ifndef __PCSX_REGS_H__
#define __PCSX_REGS_H__

#ifdef __cplusplus
extern "C" {
#endif

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
        u32 imm;
    } instr;
} PSXRec;

// GPR (general purpose registers)
typedef struct {
    u32 r[32];
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
#define psxRegs psxRegs_global

typedef struct {
    CP2DReg CP2D;
    CP2CReg CP2C;
    GPRReg  GPR;
    u32 code;
} PSXRegsGlobal;

extern PSXRegsGlobal psxRegs_global;

#define Config g_Config

#define _Rd_  g_psxRec.instr.d
#define _Rs_  g_psxRec.instr.s
#define _Rt_  g_psxRec.instr.rt
#define _Imm_ g_psxRec.instr.imm
#define _oB_  (psxRegs_global.GPR.r[_Rs_] + _Imm_)

// GTE data register macros (matching pcsx gte.c)
#define VX0  (psxRegs.CP2D.p[ 0 ].sw.l)
#define VY0  (psxRegs.CP2D.p[ 0 ].sw.h)
#define VZ0  (psxRegs.CP2D.p[ 1 ].sw.l)
#define VX1  (psxRegs.CP2D.p[ 2 ].w.l)
#define VY1  (psxRegs.CP2D.p[ 2 ].w.h)
#define VZ1  (psxRegs.CP2D.p[ 3 ].w.l)
#define VX2  (psxRegs.CP2D.p[ 4 ].w.l)
#define VY2  (psxRegs.CP2D.p[ 4 ].w.h)
#define VZ2  (psxRegs.CP2D.p[ 5 ].w.l)
#define R    (psxRegs.CP2D.p[ 6 ].b.l)
#define G    (psxRegs.CP2D.p[ 6 ].b.h)
#define B    (psxRegs.CP2D.p[ 6 ].b.h2)
#define CODE (psxRegs.CP2D.p[ 6 ].b.h3)
#define OTZ  (psxRegs.CP2D.p[ 7 ].w.l)
#define IR0  (psxRegs.CP2D.p[ 8 ].sw.l)
#define IR1  (psxRegs.CP2D.p[ 9 ].sw.l)
#define IR2  (psxRegs.CP2D.p[ 10 ].sw.l)
#define IR3  (psxRegs.CP2D.p[ 11 ].sw.l)
#define SXY0 (psxRegs.CP2D.p[ 12 ].d)
#define SX0  (psxRegs.CP2D.p[ 12 ].sw.l)
#define SY0  (psxRegs.CP2D.p[ 12 ].sw.h)
#define SXY1 (psxRegs.CP2D.p[ 13 ].d)
#define SX1  (psxRegs.CP2D.p[ 13 ].sw.l)
#define SY1  (psxRegs.CP2D.p[ 13 ].sw.h)
#define SXY2 (psxRegs.CP2D.p[ 14 ].d)
#define SX2  (psxRegs.CP2D.p[ 14 ].sw.l)
#define SY2  (psxRegs.CP2D.p[ 14 ].sw.h)
#define SXYP (psxRegs.CP2D.p[ 15 ].d)
#define SXP  (psxRegs.CP2D.p[ 15 ].sw.l)
#define SYP  (psxRegs.CP2D.p[ 15 ].sw.h)
#define SZ0  (psxRegs.CP2D.p[ 16 ].w.l)
#define SZ1  (psxRegs.CP2D.p[ 17 ].w.l)
#define SZ2  (psxRegs.CP2D.p[ 18 ].w.l)
#define SZ3  (psxRegs.CP2D.p[ 19 ].w.l)
#define RGB0 (psxRegs.CP2D.p[ 20 ].d)
#define R0   (psxRegs.CP2D.p[ 20 ].b.l)
#define G0   (psxRegs.CP2D.p[ 20 ].b.h)
#define B0   (psxRegs.CP2D.p[ 20 ].b.h2)
#define CD0  (psxRegs.CP2D.p[ 20 ].b.h3)
#define RGB1 (psxRegs.CP2D.p[ 21 ].d)
#define R1   (psxRegs.CP2D.p[ 21 ].b.l)
#define G1   (psxRegs.CP2D.p[ 21 ].b.h)
#define B1   (psxRegs.CP2D.p[ 21 ].b.h2)
#define CD1  (psxRegs.CP2D.p[ 21 ].b.h3)
#define RGB2 (psxRegs.CP2D.p[ 22 ].d)
#define R2   (psxRegs.CP2D.p[ 22 ].b.l)
#define G2   (psxRegs.CP2D.p[ 22 ].b.h)
#define B2   (psxRegs.CP2D.p[ 22 ].b.h2)
#define CD2  (psxRegs.CP2D.p[ 22 ].b.h3)
#define RES1 (psxRegs.CP2D.p[ 23 ].d)
#define MAC0 (psxRegs.CP2D.p[ 24 ].sd)
#define MAC1 (psxRegs.CP2D.p[ 25 ].sd)
#define MAC2 (psxRegs.CP2D.p[ 26 ].sd)
#define MAC3 (psxRegs.CP2D.p[ 27 ].sd)
#define IRGB (psxRegs.CP2D.p[ 28 ].d)
#define ORGB (psxRegs.CP2D.p[ 29 ].d)
#define LZCS (psxRegs.CP2D.p[ 30 ].d)
#define LZCR (psxRegs.CP2D.p[ 31 ].d)

// GTE control register macros (matching pcsx gte.c)
#define R11 (psxRegs.CP2C.p[ 0 ].sw.l)
#define R12 (psxRegs.CP2C.p[ 0 ].sw.h)
#define R13 (psxRegs.CP2C.p[ 1 ].sw.l)
#define R21 (psxRegs.CP2C.p[ 1 ].sw.h)
#define R22 (psxRegs.CP2C.p[ 2 ].sw.l)
#define R23 (psxRegs.CP2C.p[ 2 ].sw.h)
#define R31 (psxRegs.CP2C.p[ 3 ].sw.l)
#define R32 (psxRegs.CP2C.p[ 3 ].sw.h)
#define R33 (psxRegs.CP2C.p[ 4 ].sw.l)
#define TRX (psxRegs.CP2C.p[ 5 ].sd)
#define TRY (psxRegs.CP2C.p[ 6 ].sd)
#define TRZ (psxRegs.CP2C.p[ 7 ].sd)
#define L11 (psxRegs.CP2C.p[ 8 ].sw.l)
#define L12 (psxRegs.CP2C.p[ 8 ].sw.h)
#define L13 (psxRegs.CP2C.p[ 9 ].sw.l)
#define L21 (psxRegs.CP2C.p[ 9 ].sw.h)
#define L22 (psxRegs.CP2C.p[ 10 ].sw.l)
#define L23 (psxRegs.CP2C.p[ 10 ].sw.h)
#define L31 (psxRegs.CP2C.p[ 11 ].sw.l)
#define L32 (psxRegs.CP2C.p[ 11 ].sw.h)
#define L33 (psxRegs.CP2C.p[ 12 ].sw.l)
#define RBK (psxRegs.CP2C.p[ 13 ].sd)
#define GBK (psxRegs.CP2C.p[ 14 ].sd)
#define BBK (psxRegs.CP2C.p[ 15 ].sd)
#define LR1 (psxRegs.CP2C.p[ 16 ].sw.l)
#define LR2 (psxRegs.CP2C.p[ 16 ].sw.h)
#define LR3 (psxRegs.CP2C.p[ 17 ].sw.l)
#define LG1 (psxRegs.CP2C.p[ 17 ].sw.h)
#define LG2 (psxRegs.CP2C.p[ 18 ].sw.l)
#define LG3 (psxRegs.CP2C.p[ 18 ].sw.h)
#define LB1 (psxRegs.CP2C.p[ 19 ].sw.l)
#define LB2 (psxRegs.CP2C.p[ 19 ].sw.h)
#define LB3 (psxRegs.CP2C.p[ 20 ].sw.l)
#define RFC (psxRegs.CP2C.p[ 21 ].sd)
#define GFC (psxRegs.CP2C.p[ 22 ].sd)
#define BFC (psxRegs.CP2C.p[ 23 ].sd)
#define OFX (psxRegs.CP2C.p[ 24 ].sd)
#define OFY (psxRegs.CP2C.p[ 25 ].sd)
#define H   (psxRegs.CP2C.p[ 26 ].sw.l)
#define DQA (psxRegs.CP2C.p[ 27 ].sw.l)
#define DQB (psxRegs.CP2C.p[ 28 ].sd)
#define ZSF3 (psxRegs.CP2C.p[ 29 ].sw.l)
#define ZSF4 (psxRegs.CP2C.p[ 30 ].sw.l)
#define FLAG (psxRegs.CP2C.p[ 31 ].d)

// GPU callback - will be set by the test framework
extern GPUAddVertexFunc GPU_addVertex;

// Config
extern int Widescreen;

#ifdef __cplusplus
}
#endif

#endif
