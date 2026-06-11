# memcpy bug -- the plain explanation of question

If you encrypt the exact same 8-byte plaintext block over and over again:

* The first 2 bytes of the encrypted blocks will be different every single time. This is because the first 2 bytes of prev_block change after every iteration, altering the input to the next block's encryption.
* The last 6 bytes of the encrypted blocks will eventually become identical and repeat. Because memcpy never updates the last 6 bytes of prev_block, those bytes remain static. Once the initial randomness or stack data stabilizes, identical plaintexts will yield identical ciphertext chunks for those final 6 bytes. This leaks structural patterns about your file, completely breaking the core security promise of CBC mode.

------------------------------
## Bug Report: Out-of-Bounds/Truncated Element Copy in CBC Chaining Memory Block 
### Bugfix commit - 3016e745c344ce64b2ff8046136543ab2725c519
* Severity: High (Security Vulnerability & Technical Debt)
* Type: Memory Operation Mismatch / Cryptographic Flaw
* Component: processor.c (Inside encrypt() and decrypt() functions)

## Description
The block cipher implementation utilizes a memcpy operation to copy the current ciphertext block (file_word) into the feedback buffer (prev_block) to maintain the Cipher Block Chaining (CBC) state mechanism.
However, memcpy expects the final argument to represent the size of data to be moved in bytes. Instead, the codebase passes words_per_chunk (which is an element count of 32-bit integers).
On standard configurations where CHUNK_SIZE = 8 and WORD_SIZE = 32, words_per_chunk resolves to 2. As a result, memcpy copies only 2 bytes (16 bits) instead of the intended 8 bytes (64 bits).
## Root Cause Code Fragment

// Inside encrypt() and decrypt():
memcpy(prev_block, file_word, words_per_chunk); 

Actual Evaluation: memcpy(prev_block, file_word, 2);
Intended Evaluation: memcpy(prev_block, file_word, 8);
## Concrete Memory Example Trace
Consider a scenario with a CHUNK_SIZE of 8 bytes where a plaintext block is processed, resulting in a ciphertext file_word of [AA BB CC DD EE FF 11 22].

   1. Initial prev_block allocation (Little Endian representation):
   [04 03 02 01] [04 03 02 01]
   2. Execution of memcpy(prev_block, file_word, 2);:
   The operating system copies exactly 2 raw bytes and stops execution.
   3. Resulting state of prev_block memory block:
   [AA BB 02 01] [04 03 02 01]
   * Bytes 0–1: Successfully overwritten with new ciphertext feedback (AA BB).
      * Bytes 2–7: Untouched. They retain old state memory from initialization or previous operations.
   
## Impact

   1. Symmetric Masking Exception: The file decrypts successfully only because the exact same memory truncation bug is cloned inside the decryption executable loop, causing the processing error to invert itself mathematically.
   2. Cryptographic Entropy Collapse: Because 75% of the chaining block remains completely static across multiple iterations, repeating pattern sequences in raw data files will leak through the ciphertext stream, allowing pattern recognition or dictionary attacks.

------------------------------


