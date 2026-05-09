#include "processor.h"
#include "fileio.h"
#include "xtea.h"
#include "cbc.h"
#include "config.h"
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
    //packing-unpacking variables
    size_t bytes_per_word = (WORD_SIZE)/(BITS_PER_BYTE); 
    size_t words_per_chunk =(CHUNK_SIZE)/(bytes_per_word);
    uint32_t file_word[words_per_chunk];
    uint8_t last_block_flag=0;//padding variable
    //packing-unpacking variables

    //cbc variables
    uint32_t prev_block[words_per_chunk];//=IV;
    //cbc variables

    //temp_key
    uint32_t key[KEY_SIZE/WORD_SIZE];//call keygen here 
    //

    //fileio vars
    uint8_t file_chunk[CHUNK_SIZE];

    size_t bytes_read,bytes_written;

    FILE * fpr = file_open_read(file_info.filepath_in);
    if(fpr==NULL){printf("Error finding \"%s\".\n",file_info.filepath_in);return;}

    FILE * fpw = file_open_write(file_info.filepath_out);
    if(fpw==NULL){printf("Error finding \"%s\".\n",file_info.filepath_out);fclose(fpr);return;}
    //fileio vars

    printf("\nBoth locations found.\n");

    do
    {
        bytes_read = file_chunk_read(fpr,CHUNK_SIZE,file_chunk);
        //PKCS#7 padding
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
        bytes_to_words(file_chunk, file_word, bytes_per_word);
        //PKCS#7 padding

        //cbc
        xor_block(file_word,prev_block);
        
        //xtea encryption
        xtea_encrypt(file_word, key);
        //xtea encryption

        for(int i = 0; i < words_per_chunk;i++){
            prev_block[i] = file_word[i];
        }
        //cbc

        words_to_bytes(file_word,file_chunk, bytes_per_word);

        bytes_written = file_chunk_write(fpw,CHUNK_SIZE,file_chunk);

    }while(bytes_read==CHUNK_SIZE);

    //attach Header and Tag
    fclose(fpr);
    fclose(fpw);

    printf("\nThe encrypted file is written at location \"%s\".\n",file_info.filepath_out);
    return;
}

void decrypt(struct file_info_st file_info){
    //packing-unpacking variables
    size_t bytes_per_word = (WORD_SIZE)/(BITS_PER_BYTE);
    size_t words_per_chunk =(CHUNK_SIZE)/(bytes_per_word);
    uint32_t file_word[words_per_chunk];
    //packing-unpacking variables

    //cbc variables
    uint32_t prev_block[words_per_chunk];//=IV
    uint32_t prev_block_buffer[words_per_chunk];
    //cbc variables

    //temp_key
    uint32_t key[KEY_SIZE/WORD_SIZE];    
    //

    //fileio vars
    uint8_t file_chunk[CHUNK_SIZE];
    size_t bytes_read,bytes_written;

    FILE * fpr = file_open_read(file_info.filepath_in);
    if(fpr==NULL){printf("Error finding \"%s\".\n",file_info.filepath_in);return;}

    FILE * fpw = file_open_write(file_info.filepath_out);
    if(fpw==NULL){printf("Error finding \"%s\".\n",file_info.filepath_out);fclose(fpr);return;}
    //fileio vars
    printf("\nBoth locations found.\n");

    do
    {
        bytes_read = file_chunk_read(fpr,CHUNK_SIZE,file_chunk);
        if(bytes_read==0){break;}

        bytes_to_words(file_chunk, file_word, bytes_per_word); 
        //cbc
        for(int i = 0; i < words_per_chunk;i++){
            prev_block_buffer[i]=file_word[i];
        }
        //xtea decryption
        xtea_decrypt(file_word, key);
        //xtea decryption

        xor_block(file_word,prev_block);
        for(int i = 0; i < words_per_chunk;i++){
            prev_block[i]=prev_block_buffer[i];
        }
        //cbc

        words_to_bytes(file_word,file_chunk, bytes_per_word);

        //depad
        bytes_written = file_chunk_write(fpw,bytes_read,file_chunk);
    } while(bytes_read==CHUNK_SIZE);
    
    fclose(fpr);
    fclose(fpw);

    printf("\nThe decrypted file is written at location \"%s\".\n",file_info.filepath_out);
    return;
}

void bytes_to_words(uint8_t file_chunk[], uint32_t file_word[], size_t bytes_per_word)
{
    for (size_t i = 0; i < (CHUNK_SIZE/bytes_per_word);i++){
        for(size_t j = 0; j < bytes_per_word; j++){
            file_word[i] <<= BITS_PER_BYTE;
            file_word[i]|=(uint32_t)file_chunk[(i * bytes_per_word)+j];
        }

    }
}


void words_to_bytes(const uint32_t file_word[], uint8_t file_chunk[], size_t bytes_per_word)
{
    for (size_t i = 0; i < (CHUNK_SIZE / bytes_per_word); i++)
    {
        /* local copy because we shift it repeatedly */
        uint32_t current_word = file_word[i];

        /* reverse index because LSB extracted first */
        for (int j = bytes_per_word - 1; j >= 0; j--)
        {
            /* store lowest byte */
            file_chunk[(bytes_per_word * i) + j] = (uint8_t)current_word;

            /* expose next byte */
            current_word >>= BITS_PER_BYTE;
        }
    }
}