````markdown id="k4l5aa"
# Pack / Unpack Unit Test

## Purpose

This unit test validates the correctness of the byte packing and unpacking layer used by the encryption pipeline.

The packing layer acts as the bridge between:

```text
File I/O byte stream
and
XTEA 32-bit block operations
````

Because XTEA operates on:

```text
uint32_t block[2]
```

while file data is processed as:

```text
uint8_t file_chunk[8]
```

conversion between byte arrays and 32-bit words is required.

---

# Functions Tested

## bytes_to_words()

Converts:

```text
uint8_t[8]
```

into:

```text
uint32_t[2]
```

using big-endian packing.

---

## words_to_bytes()

Converts:

```text
uint32_t[2]
```

back into:

```text
uint8_t[8]
```

restoring the original byte sequence.

---

# Test Objective

Validate that:

```text
bytes
→ words
→ bytes
```

produces identical output.

This is called a:

```text
round-trip validation test
```

and is the most reliable correctness check for reversible serialization logic.

---

# Test Input

The following byte sequence is used:

```text
01 02 03 04 05 06 07 08
```

Represented as:

```c
uint8_t input_chunk[8] = {
    0x01,0x02,0x03,0x04,
    0x05,0x06,0x07,0x08
};
```

---

# Expected Packed Output

Big-endian packing should produce:

```text
word[0] = 0x01020304
word[1] = 0x05060708
```

---

# Expected Reconstruction

After unpacking:

```text
01 02 03 04 05 06 07 08
```

must be restored exactly.

Even a one-byte mismatch indicates conversion failure.

---

# Packing Design Used

The implementation uses a:

```text
streaming builder approach
```

instead of positional assembly.

Conceptually:

```text
shift current word left
insert next byte
repeat
```

Example:

```text
0
→ 01
→ 0102
→ 010203
→ 01020304
```

---

# Why Streaming Packing Was Chosen

Advantages:

* simpler logic
* avoids complex shift-position formulas
* no dependency on pre-zeroed arrays
* cleaner symmetry with unpacking
* easier to reason about

---

# Unpacking Design

The unpacker uses a:

```text
streaming peel-off approach
```

Conceptually:

```text
extract lowest byte
shift word right
repeat
```

while writing bytes in reverse index order to preserve big-endian layout.

---

# Validation Output

Observed output:

```text
input bytes:1
input bytes:2
input bytes:3
input bytes:4
input bytes:5
input bytes:6
input bytes:7
input bytes:8

Packed word: 1020304
Packed word: 5060708

Reconstructed bytes:1
Reconstructed bytes:2
Reconstructed bytes:3
Reconstructed bytes:4
Reconstructed bytes:5
Reconstructed bytes:6
Reconstructed bytes:7
Reconstructed bytes:8

SUCCESS
```

---

# Result

The test passed successfully.

This confirms:

* correct big-endian packing
* correct unpacking
* reversible transformation
* compatibility with XTEA block structure

---

# Importance In Project

This module is foundational for the encryption pipeline.

It enables:

```text
file bytes
↔ uint32_t cipher blocks
↔ XTEA encryption/decryption
```

Without correct serialization logic, block cipher operations would corrupt file data.

---

# Current Status

Packing and unpacking layer successfully validated and integrated into the project architecture.

```
```
