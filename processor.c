#include "processor.h"
#include "fileio.h"
#include "xtea.h"
struct file_info_st get_file_info(){
    struct file_info_st file_info;
    printf("Enter the input File Path.\n");
    fgets(file_info.filepath_in,256,stdin);
    file_info.filepath_in[strcspn(file_info.filepath_in,"\n")]='\0';
    printf("Enter the output File Path.\n");
    fgets(file_info.filepath_out,256,stdin);
    file_info.filepath_out[strcspn(file_info.filepath_out,"\n")]='\0';
    printf("Enter Password.\n");
    fgets(file_info.password,128,stdin);
    file_info.password[strcspn(file_info.password,"\n")]='\0';
    return file_info;
}

void encrypt(struct file_info_st file_info){

    unsigned char file_chunk[CHUNK_SIZE];
    size_t bytes_read,bytes_written;
    int last_block_flag=0;

    FILE * fpr = file_open_read(file_info.filepath_in);
    if(fpr==NULL){printf("Error finding \"%s\".\n",file_info.filepath_in);return;}

    FILE * fpw = file_open_write(file_info.filepath_out);
    if(fpw==NULL){printf("Error finding \"%s\".\n",file_info.filepath_out);fclose(fpr);return;}

    printf("\nBoth locations found.\n");

    do
    {
        bytes_read = file_chunk_read(fpr,CHUNK_SIZE,file_chunk);

        if(bytes_read==0 && last_block_flag==1){break;}
        else if(bytes_read==0 && last_block_flag==0){
            for(int i=0;i<CHUNK_SIZE;i++){
            file_chunk[i]=CHUNK_SIZE;
            }
            last_block_flag=1;
        }
        else if(bytes_read<CHUNK_SIZE){
            int padding =(CHUNK_SIZE - bytes_read);
            for(int i=1;i<=padding;i++){file_chunk[CHUNK_SIZE-i]=padding;
            }
            last_block_flag=1;
        }

        //call keygen here
        //call xtea encryption here
        bytes_written = file_chunk_write(fpw,CHUNK_SIZE,file_chunk);
    }while(bytes_read==CHUNK_SIZE);

    fclose(fpr);
    fclose(fpw);

    printf("\nThe encrypted file is written at location \"%s\".\n",file_info.filepath_out);
    return;
}

void decrypt(struct file_info_st file_info){
    unsigned char file_chunk[CHUNK_SIZE];
    size_t bytes_read,bytes_written;

    FILE * fpr = file_open_read(file_info.filepath_in);
    if(fpr==NULL){printf("Error finding \"%s\".\n",file_info.filepath_in);return;}

    FILE * fpw = file_open_write(file_info.filepath_out);
    if(fpw==NULL){printf("Error finding \"%s\".\n",file_info.filepath_out);fclose(fpr);return;}

    printf("\nBoth locations found.\n");

    do
    {
        bytes_read = file_chunk_read(fpr,CHUNK_SIZE,file_chunk);
        if(bytes_read==0){break;}
        //depad
        //call keygen here
        //call xtea decryption here
        bytes_written = file_chunk_write(fpw,bytes_read,file_chunk);
    } while(bytes_read==CHUNK_SIZE);

    fclose(fpr);
    fclose(fpw);

    printf("\nThe decrypted file is written at location \"%s\".\n",file_info.filepath_out);
    return;
}
