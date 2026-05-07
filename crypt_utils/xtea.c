#include "xtea.h"
void xtea_encrypt(uint32_t block[2],const uint32_t key[4]){
    uint32_t v0 = block[0];
    uint32_t v1 = block[1];
    uint32_t sum = 0;

    for(int i = 0;i < XTEA_ROUNDS;i++){
        v0 += (((v1 << 4)^(v1 >> 5)) + v1)^(sum + key[sum & 3]);
        sum += DELTA;
        v1 += (((v0 << 4)^(v0 >> 5)) + v0)^(sum + key[(sum >> 11) & 3]);
    }
    block[0]=v0;
    block[1]=v1;
}

void xtea_decrypt(uint32_t block[2],const uint32_t key[4]){
    uint32_t v0 = block[0];
    uint32_t v1 = block[1];
    uint32_t sum = DELTA * XTEA_ROUNDS;

    for(int i = 0;i < XTEA_ROUNDS;i++){
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= DELTA;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    block[0] = v0;
    block[1] = v1;
}