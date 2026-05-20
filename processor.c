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
    //packing-unpacking variables

    //padding variables
    uint8_t padding=0;
    uint8_t last_block_flag=0;
    //padding variable

    //cbc variables
    uint32_t prev_block[words_per_chunk];//=IV;
    for(int i=0;i<words_per_chunk;i++){prev_block[i]=0x01020304;}
    //cbc variables

    //temp_key
    uint32_t key[KEY_SIZE/WORD_SIZE];//call keygen here 
    for(int i=0;i<(KEY_SIZE/WORD_SIZE);i++){key[i]=0x01020304;}
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
            padding =(CHUNK_SIZE - bytes_read);
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

        memcpy(prev_block,file_word, words_per_chunk * bytes_per_word);
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

    //depadding variables
    uint8_t padding=0;
    uint8_t last_block_flag=0;
    uint8_t depad_buffer[CHUNK_SIZE];
    //depadding variable

    //cbc variables
    uint32_t prev_block[words_per_chunk];//=IV
    for(int i=0;i<words_per_chunk;i++){prev_block[i]=0x01020304;}
    uint32_t prev_block_buffer[words_per_chunk];
    //cbc variables

    //temp_key
    uint32_t key[KEY_SIZE/WORD_SIZE];//extract key from password here
    for(int i=0;i<(KEY_SIZE/WORD_SIZE);i++){key[i]=0x01020304;}   
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

    bytes_read = file_chunk_read(fpr,CHUNK_SIZE,file_chunk);
    do
    {
        if(bytes_read==0 && last_block_flag==1){break;}

        bytes_to_words(file_chunk, file_word, bytes_per_word); 
        //cbc
        memcpy(prev_block_buffer,file_word, words_per_chunk * bytes_per_word);
        //xtea decryption
        xtea_decrypt(file_word, key);
        //xtea decryption

        xor_block(file_word,prev_block);
        memcpy(prev_block,prev_block_buffer, words_per_chunk * bytes_per_word);
        //cbc

        words_to_bytes(file_word, file_chunk, bytes_per_word);
        memcpy(depad_buffer, file_chunk, CHUNK_SIZE);

        //depad
        bytes_read = file_chunk_read(fpr,CHUNK_SIZE,file_chunk);
        if(bytes_read==0 && last_block_flag==0){
            padding = depad_buffer[CHUNK_SIZE - 1];
            for(uint8_t i=0; i < (CHUNK_SIZE-padding);i++){
                fwrite((depad_buffer + i),1,1,fpw);
                last_block_flag=1;
            }
        }
        //depad

        bytes_written = file_chunk_write(fpw,bytes_read,depad_buffer);
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