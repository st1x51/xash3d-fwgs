#ifndef STUB_PSP_H
#define STUB_PSP_H

#include "common.h"
#include <fcntl.h> 

int gethostname(char *name, size_t len);
#define FIONBIO 0x8004667e
#define __builtin_allegrex_wsbw __builtin_bswap32
#define __builtin_allegrex_wsbh __builtin_bswap16

#endif