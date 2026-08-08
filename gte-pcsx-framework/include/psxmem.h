#ifndef __PCSX_MEM_MOCK_H__
#define __PCSX_MEM_MOCK_H__

#include "pcsx_types.h"

static inline u32 psxMemRead32(u32 addr) {
    (void)addr;
    return 0;
}

static inline void psxMemWrite32(u32 addr, u32 value) {
    (void)addr; (void)value;
}

#endif
