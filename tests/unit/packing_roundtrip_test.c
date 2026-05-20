/*======================================== 
TEST ID : UNIT-PACK-01 
TEST NAME : Packing-Unpacking Roundtrip Unit
========================================
Validate Packing bytes_to_words and unpacking words_bytes
*/

#include <stdint.h>
#include <stdio.h>
#include "../../config.h"
#include "../../processor.h"

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

    /* pack */
    bytes_to_words(
        input_chunk,
        packed_words,
        bytes_per_word);

    /* unpack */
    words_to_bytes(
        packed_words,
        output_chunk,
        bytes_per_word
    );
    if(memcmp(input_chunk,output_chunk,CHUNK_SIZE)==0){
        puts("\nSTATUS: PASS");
        return 0;
    }else{
        puts("\nSTATUS: FAIL");
        return 1;
    }
}