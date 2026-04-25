#include <stdio.h>
struct file_info_st{
    char filepath_in[256],filepath_out[256],password[128];
};
FILE * file_open_read(char filepath_in[]);
FILE * file_open_write(char filepath_out[]);
size_t file_chunk_read(FILE * fp,size_t chunk_size,unsigned char file_chunk[]);
size_t file_chunk_write(FILE * fp,size_t chunk_size,const unsigned char file_chunk[]);