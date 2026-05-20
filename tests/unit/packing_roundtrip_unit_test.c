#include <stdint.h>
#include <stdio.h>
#include "../../config.h"
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

int main()
{
    size_t bytes_per_word = (WORD_SIZE)/(BITS_PER_BYTE);
    /* input buffer */
    uint8_t input_chunk[CHUNK_SIZE] = {
        0x01, 0x02, 0x03, 0x04,
        0x05, 0x06, 0x07, 0x08
    };

    /* packed output */
    uint32_t packed_words[CHUNK_SIZE / bytes_per_word];

    /* reconstructed bytes */
    uint8_t output_chunk[CHUNK_SIZE];

    for(int i = 0; i < CHUNK_SIZE; i++ ){
        printf("input bytes:%u\n",input_chunk[i]);
    }

    /* pack */
    bytes_to_words(
        input_chunk,
        packed_words,
        bytes_per_word);

    /* display packed words */
    for(int i = 0; i < (CHUNK_SIZE/bytes_per_word); i++ ){
        printf("Packed word: %X\n",packed_words[i]);
    }

    /* unpack */
    words_to_bytes(
        packed_words,
        output_chunk,
        bytes_per_word
    );

    /* display reconstructed bytes */
    for(int i = 0; i < CHUNK_SIZE; i++ ){
        printf("Reconstructed bytes:%u\n",output_chunk[i]);
    }

    /* compare original vs reconstructed */
    for(int i = 0; i < CHUNK_SIZE; i++ ){
        if(input_chunk[i]==output_chunk[i]){
           continue;
        }
        printf("TEST FAILED");
    }
    printf("SUCCESS");
    return 0;
}