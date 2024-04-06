#ifndef UTILS_H
#define UTILS_H

#include "types.h"

u32 aton(s8 *ip);

s8 *ntoa(u32 addr);

u16 checksum16(u16 *data, s32 len);

#endif // UTILS_H