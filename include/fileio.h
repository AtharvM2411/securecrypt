#ifndef FILEIO_H
#define FILEIO_H
#include <stdio.h>
#include <stdint.h>
typedef struct file_info_st{
    char filepath_in[256],filepath_out[256],password[128];
} file_info_st;
FILE * file_open_read(char filepath_in[]);
FILE * file_open_write(char filepath_out[]);
size_t file_chunk_read(FILE * fp,size_t chunk_size,uint8_t file_chunk[]);
size_t file_chunk_write(FILE * fp,size_t chunk_size,const uint8_t file_chunk[]);
#endif //FILEIO_H