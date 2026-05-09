#include "cbc.h"
void xor_block(uint32_t block_plain[2], const uint32_t block_prev[2]){
    for(int i = 0; i < (CHUNK_SIZE/(WORD_SIZE/BITS_PER_BYTE));i++){
    block_plain[i] ^= block_prev[i];
    }
}