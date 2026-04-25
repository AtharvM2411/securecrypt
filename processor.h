#define CHUNK_SIZE 8
#include <string.h>
#include"fileio.h"
struct file_info_st get_file_info();
void encrypt(struct file_info_st file_info);
void decrypt(struct file_info_st file_info);