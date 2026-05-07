````markdown id="n7m4aa"
# Packing / Unpacking Layer Documentation

## Purpose

These helper functions act as the bridge between:

```text
File Input / Output Layer
(raw bytes)

        ↕

XTEA Cipher Layer
(uint32_t words)
````

Your `processor.c` reads file data as:

```c
uint8_t file_chunk[CHUNK_SIZE];
```

But XTEA operates on:

```c
uint32_t block[2];
```

Because XTEA encrypts:

* 64-bit block total
* split into two 32-bit halves

So conversion is required.

---

# Where It Is Used In Project

## Encryption Flow

```text
Read 8 bytes from file
↓
bytes_to_words()
↓
xtea_encrypt()
↓
words_to_bytes()
↓
Write encrypted 8 bytes
```

## Decryption Flow

```text
Read encrypted 8 bytes
↓
bytes_to_words()
↓
xtea_decrypt()
↓
words_to_bytes()
↓
Write plaintext bytes
```

---

# Why Conversion Is Needed

Files store data byte-by-byte:

```text
41 42 43 44 45 46 47 48
```

But XTEA arithmetic uses:

```text
uint32_t left_half
uint32_t right_half
```

So bytes must be packed into words before encryption,
then unpacked after encryption.

---

# Chosen Byte Order

## Big-Endian

Bytes:

```text
11 22 33 44
```

Become:

```text
0x11223344
```

This was chosen because it is explicit and easy to reason about.

---

# bytes_to_words()

## Function Role

Converts:

```text
8 bytes
```

into:

```text
2 uint32_t words
```

Example:

```text
01 02 03 04 05 06 07 08
```

Becomes:

```text
word[0] = 0x01020304
word[1] = 0x05060708
```

---

# Packing Approach Used

## Approach 1: Positional Assembly (Implemented)

Each byte is moved directly into final location.

```text
byte0 << 24
byte1 << 16
byte2 << 8
byte3 << 0
```

Then combined using:

```c
|=
```

### Why `|=` Is Used

Because each shifted byte occupies different bit positions.

So they are merged into one final 32-bit word.

---

# Alternative Packing Approach (Discussed)

## Approach 2: Streaming Builder

Instead of calculating final positions:

```c
word = 0;
word = (word << 8) | byte;
```

Example:

```text
0
<<8 + 11 = 11
<<8 + 22 = 1122
<<8 + 33 = 112233
<<8 + 44 = 11223344
```

### Pros

* Cleaner symmetry with unpacking
* No shift-position formula
* Elegant logic

### Pros of Current Implemented Approach

* Explicit byte placement
* Easier to visualize exact positions
* Good for learning binary layout

Both are valid.

---

# Important Note

Because implemented version uses:

```c
|=
```

Destination array must start as zero.

Example:

```c
uint32_t file_word[2] = {0};
```

---

# words_to_bytes()

## Function Role

Converts:

```text
2 uint32_t words
```

back into:

```text
8 bytes
```

Example:

```text
0x11223344
```

Becomes:

```text
11 22 33 44
```

---

# Unpacking Approach Used

## Streaming Peel-Off Method

Take lowest byte repeatedly:

```text
current_word = 0x11223344

store 44
shift right 8

store 33
shift right 8

store 22
shift right 8

store 11
```

Because lowest byte comes first, output indexes are filled in reverse:

```text
3,2,1,0
```

This restores big-endian byte order.

---

# Why Reverse Loop Was Chosen

If increment loop were used:

```text
0,1,2,3
```

Then extracting bytes would need:

```text
word >> 24
word >> 16
word >> 8
word >> 0
```

Meaning:

* more shift calculations
* need untouched original word
* less elegant flow

Current countdown method is cleaner.

---

# Why Signed int Used For Reverse Loop

Do NOT use:

```c
size_t j
```

for:

```c
j >= 0
```

Because `size_t` is unsigned and may underflow.

Use:

```c
int j
```

instead.

---

# Why Local Copy Is Used

```c
uint32_t current_word = file_word[i];
```

Because unpacking repeatedly shifts the value.

Original input array should remain unchanged.

---

# Round-Trip Validation Test

Best correctness test:

```text
Original bytes
↓
bytes_to_words()
↓
words_to_bytes()
↓
Final bytes
```

If final bytes equal original bytes exactly,
the conversion layer is correct.

---

# Why This Module Is Important

Once packing/unpacking works correctly:

```text
file bytes ↔ cipher words
```

is permanently solved.

Then integrating XTEA into `processor.c` becomes straightforward.

---

# Final Summary

This module is not encryption itself.

It is the **translation layer** between:

```text
Storage Format (bytes)
and
Cipher Format (32-bit words)
```

Without it, XTEA cannot operate directly on file chunks.

```
```
