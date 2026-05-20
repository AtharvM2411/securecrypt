#include <stdio.h> 
#include "../crypt_utils/xtea.h"
int main(){ 
    uint32_t block[] = {0x12345678, 0x9ABCDEF0};
    
    uint32_t key[] = {
    0x11111111,
    0x22222222,
    0x33333333,
    0x44444444
};
xtea_encrypt(block,key);
xtea_decrypt(block,key);

}