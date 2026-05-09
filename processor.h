#include <string.h>
#include"fileio.h"
struct file_info_st get_file_info();
void encrypt(struct file_info_st file_info);
void decrypt(struct file_info_st file_info);
void bytes_to_words(uint8_t file_chunk[],uint32_t file_word[],size_t bytes_per_word);
void words_to_bytes(const uint32_t file_word[],uint8_t file_chunk[],size_t bytes_per_word);