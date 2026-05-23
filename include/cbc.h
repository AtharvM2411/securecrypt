#ifndef CBC_H
#define CBC_H
#include <stdint.h>
#include "config.h"
void xor_block(uint32_t block_plain[2],const uint32_t block_prev[2]);
#endif //CBC_H
