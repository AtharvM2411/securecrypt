#include <stdio.h>
struct file_info_st{
    char filepath_in[256],filepath_out[256],password[128];
};
struct file_info_st get_file_info();
void encrypt();
void decrypt();