/*======================================== 
TEST ID : INT-FILE-XTEA-PKCS#7-CBC-01 
TEST NAME : FileIO,XTEA,PKCS#7,CBC Integration 
========================================
Manually Validate fileio, encrypt-decrypt roundtrip, padding-deppading,CBC
*/

#include <stdio.h>
#include "processor.h"

int main(){
    int choice=0;
    struct file_info_st file_info_enc;
    strcpy(file_info_enc.filepath_in,"tests\\integration\\INT-FILE-XTEA-PACK-PKCS#7-CBC-01\\input\\input.txt");
    strcpy(file_info_enc.filepath_out,"tests\\integration\\INT-FILE-XTEA-PACK-PKCS#7-CBC-01\\encrypted\\encrypted.enc");
    strcpy(file_info_enc.password,"12345");
    encrypt(file_info_enc);
    
    struct file_info_st file_info_dec;
    strcpy(file_info_dec.filepath_in,"tests\\integration\\INT-FILE-XTEA-PACK-PKCS#7-CBC-01\\encrypted\\encrypted.enc");
    strcpy(file_info_dec.filepath_out,"tests\\integration\\INT-FILE-XTEA-PACK-PKCS#7-CBC-01\\decrypted\\decrypted.txt");
    strcpy(file_info_dec.password,"12345");

    decrypt(file_info_dec);
    printf("\nEnter the Status, 0 for PASS, 1 for FAIL\n");
    scanf("%d",&choice);
    if (choice==0){
        puts("\nSTATUS: PASS");
    }else{puts("\nSTATUS: FAIL");}
    return 0;
}