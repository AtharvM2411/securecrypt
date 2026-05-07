#include <stdint.h>
#define XTEA_ROUNDS 32
#define DELTA 0x9E3779B9u
void xtea_encrypt(uint32_t block[2], const uint32_t key[4]);
void xtea_decrypt(uint32_t block[2], const uint32_t key[4]);