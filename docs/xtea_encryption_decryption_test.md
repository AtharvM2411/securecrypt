# XTEA Encryption / Decryption Validation Test

## Purpose

This test validates the correctness of the implemented:

- `xtea_encrypt()`
- `xtea_decrypt()`

functions.

The objective is to confirm that:

```text
Plaintext
→ Encrypt
→ Ciphertext
→ Decrypt
→ Original Plaintext Restored