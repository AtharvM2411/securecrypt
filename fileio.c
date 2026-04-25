#include "fileio.h"
FILE * file_open_read(char filepath_in[]){
    FILE *fp = fopen(filepath_in,"rb");
    if(fp==NULL){return NULL;}
    else{return fp;}
}

FILE * file_open_write(char filepath_out[]){
    FILE *fp = fopen(filepath_out,"wb");
    if(fp==NULL){return NULL;}
    else{return fp;}
}

size_t file_chunk_read(FILE * fp,size_t chunk_size,unsigned char file_chunk[]){
    return fread(file_chunk,1,chunk_size,fp);
}

size_t file_chunk_write(FILE * fp,size_t chunk_size,const unsigned char file_chunk[]){
    return fwrite(file_chunk,1,chunk_size,fp);
}