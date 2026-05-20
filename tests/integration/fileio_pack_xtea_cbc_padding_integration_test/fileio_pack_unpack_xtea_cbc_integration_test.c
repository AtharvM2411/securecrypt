#include <stdio.h>
#include "processor.h"
int main(){
    struct file_info_st file_info_enc;
    strcpy(file_info_enc.filepath_in,"tests\\integration_tests\\fileio_pack_xtea_cbc_padding_integration_test\\input\\input.txt");
    strcpy(file_info_enc.filepath_out,"tests\\integration_tests\\fileio_pack_xtea_cbc_padding_integration_test\\encrypted\\encrypted.txt");
    strcpy(file_info_enc.password,"12345");
    encrypt(file_info_enc);
    
    struct file_info_st file_info_dec;
    strcpy(file_info_dec.filepath_in,"tests\\integration_tests\\fileio_pack_xtea_cbc_padding_integration_test\\encrypted\\encrypted.txt");
    strcpy(file_info_dec.filepath_out,"tests\\integration_tests\\fileio_pack_xtea_cbc_padding_integration_test\\decrypted\\decrypted.txt");
    strcpy(file_info_dec.password,"12345");

    decrypt(file_info_dec);
}