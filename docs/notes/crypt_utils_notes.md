# File Encryption Software: Master Implementation Sheet (Enhanced Hybrid Framework)

This sheet outlines a secure file encryption system built from scratch in C using:

- XTEA (Block Cipher)
- SHA-256 (Hash Function)
- HMAC-SHA256 (Authentication)
- PBKDF2-HMAC-SHA256 (Password Key Derivation)
- CBC Mode
- PKCS#7 Padding

Project Utility Folder:

```text
crypt_utils/
````

Recommended structure:

```text
project/
│── main.c
│── processor.h
│── processor.c
│── fileio.c
│── fileio.h
│
└── crypt_utils/
    │── xtea.c
    │── xtea.h
    │── sha256.c
    │── sha256.h
    │── hmac.c
    │── hmac.h
    │── pbkdf2.c
    │── pbkdf2.h
    │── cbc.c
    │── cbc.h
    │── padding.c
    │── padding.h
    │── random.c
    │── random.h
    │── secure_compare.c
    └── secure_compare.h
```

---

# Phase 1: The "Engine" Layer (Core Primitives)

Build these low-level cryptographic engines first.

---

## 1. XTEA Block Cipher

* Encrypts fixed 8-byte blocks
* Uses 128-bit key
* 32 cycles (64 Feistel rounds)

Functions:

```c
void xtea_encrypt(uint32_t v[2], uint32_t key[4]);
void xtea_decrypt(uint32_t v[2], uint32_t key[4]);
```

---

## 2. SHA-256

Cryptographic hash function.

* Input: Any length data
* Output: 32-byte digest

```c
void sha256(
    const uint8_t *msg,
    size_t len,
    uint8_t hash[32]
);
```

---

## 3. HMAC-SHA256

Keyed authentication construction.

Formula:

```text
H(K XOR opad || H(K XOR ipad || message))
```

Constants:

```text
ipad = 0x36
opad = 0x5C
```

Function:

```c
void hmac_sha256(
    const uint8_t *key,
    int keylen,
    const uint8_t *msg,
    int msglen,
    uint8_t out[32]
);
```

---

# Phase 2: The "Key" Layer (PBKDF2)

## Goal

Convert user password into strong binary keys.

---

## Inputs

* Password
* Salt (16 random bytes)
* Iterations (100000 recommended)

---

## PBKDF2 Workflow

### Block 1:

```text
U1 = HMAC(password, salt || 0x00000001)
T = U1
```

### Loop:

For i = 2 to iterations:

```text
Ui = HMAC(password, U(i-1))
T = T XOR Ui
```

---

## Final Output

Derive **32 bytes total**

Split into two independent keys:

```text
Bytes 0..15   = Encryption Key (EncKey)
Bytes 16..31  = MAC Key (MacKey)
```

---

## Why Key Separation?

Better security design.

Do NOT reuse same key for:

* Encryption
* Authentication

---

# Phase 3: The "Encryption" Layer (XTEA-CBC)

## Goal

Encrypt full files securely using block chaining.

---

# 1. Padding (PKCS#7)

XTEA block size = 8 bytes

If plaintext size is not multiple of 8:

```text
N = 8 - (len % 8)
Append N bytes each equal to N
```

Example:

```text
HELLO
```

5 bytes becomes:

```text
48 45 4C 4C 4F 03 03 03
```

---

# 2. IV (Initialization Vector)

Generate random 8-byte IV.

Different IV every encryption.

---

# 3. CBC Logic

## First Block

```text
C1 = Encrypt(P1 XOR IV)
```

## Next Blocks

```text
Cn = Encrypt(Pn XOR C(n-1))
```

---

# Decryption

```text
P1 = Decrypt(C1) XOR IV
Pn = Decrypt(Cn) XOR C(n-1)
```

---

# Phase 4: The "Authentication" Layer (HMAC)

## Goal

Detect:

* Wrong password
* File corruption
* Bit flipping
* Tampering

---

## Data To Authenticate

Use entire metadata + encrypted payload:

```text
MAGIC || VERSION || ITERATIONS || SALT || IV || CIPHERTEXT
```

---

## Tag Creation

```text
TAG = HMAC-SHA256(MacKey, data)
```

Output:

```text
32 bytes
```

---

# Decryption Rule

## VERIFY TAG FIRST

Only decrypt if HMAC matches.

---

# Secure Tag Comparison

Use constant-time compare.

Bad:

```c
memcmp(a,b,32);
```

Good:

```c
diff |= a[i] ^ b[i];
```

---

# Phase 5: File Format (Storage Layout)

Write encrypted file in this order:

| Offset | Field      | Size     | Purpose                |
| ------ | ---------- | -------- | ---------------------- |
| 0      | MAGIC      | 4 Bytes  | File identifier        |
| 4      | VERSION    | 1 Byte   | Format version         |
| 5      | ITERATIONS | 4 Bytes  | PBKDF2 rounds          |
| 9      | SALT       | 16 Bytes | Recreate keys          |
| 25     | IV         | 8 Bytes  | CBC start value        |
| 33     | CIPHERTEXT | Variable | Encrypted data         |
| End    | HMAC TAG   | 32 Bytes | Integrity verification |

---

# Suggested MAGIC Value

```text
XTF1
```

(XTEA Tool Format v1)

---

# Full Encryption Workflow

```text
1. Read plaintext file
2. User enters password
3. Generate random Salt
4. PBKDF2 -> EncKey + MacKey
5. Generate random IV
6. Apply PKCS#7 padding
7. Encrypt using XTEA-CBC
8. Compute HMAC over header + ciphertext
9. Save final file
```

---

# Full Decryption Workflow

```text
1. Read encrypted file
2. Parse header
3. Ask password
4. PBKDF2 using stored Salt
5. Recreate EncKey + MacKey
6. Verify HMAC first
7. If valid -> decrypt CBC
8. Remove padding
9. Save original file
```

---

# Security Features Achieved

* Password-based encryption
* Salted KDF
* Brute-force slowdown
* Random IV
* CBC semantic security
* Tamper detection
* Wrong password detection
* Future versioning support

---

# Recommended Utility Modules Inside crypt_utils

```text
xtea            -> block cipher
sha256          -> hash engine
hmac            -> authentication
pbkdf2          -> key derivation
cbc             -> chaining mode
padding         -> PKCS7
random          -> salt + IV generation
secure_compare  -> constant-time checks
```

---

# Future Upgrades

## Version 2 Ideas

* AES-256-CBC
* ChaCha20-Poly1305
* Argon2id KDF
* Folder encryption
* GUI
* Progress bars
* Multi-thread PBKDF2

---

# Final Summary

```text
Password
   ↓
PBKDF2
   ↓
EncKey + MacKey
   ↓
XTEA-CBC Encrypt
   ↓
HMAC Authenticate
   ↓
[MAGIC][HEADER][DATA][TAG]
```

This architecture transforms the project from a student demo into a serious cryptography engineering project.