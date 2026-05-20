# SecureCrypt Testing Plan

Version: 1.0
Project: Secure File Encryption and Decryption Tool using XTEA

---

# 1. Purpose

This document defines:

* testing architecture
* testing conventions
* validation rules
* logging structure
* PASS/FAIL criteria
* reproducibility requirements
* naming standards

The testing infrastructure exists to build measurable confidence in the correctness and stability of the cryptographic system.

---

# 2. Testing Philosophy

Testing follows the progression:

```text
Unit
→ Integration
→ Security
→ Stress
```

Validation complexity must increase gradually.

Small deterministic tests must precede large system-level tests.

---

# 3. Testing Laws

## LAW 1 — Deterministic Inputs

All early-stage tests must use:

* fixed keys
* fixed IVs
* fixed plaintexts
* fixed ciphertext vectors

unless randomness itself is being tested.

Deterministic failures are easier to debug and reproduce.

---

## LAW 2 — Single Responsibility

Each test must validate one primary behavior.

BAD:

```text
testing CBC + SHA256 + HMAC simultaneously
```

GOOD:

```text
testing CBC chaining only
```

---

## LAW 3 — Independent Execution

Tests must not depend on previous tests.

Each test must:

* create its own environment
* prepare its own inputs
* generate its own outputs
* clean temporary artifacts if necessary

---

## LAW 4 — Exact Validation

Validation must always be:

```text
byte-for-byte exact
```

Visual inspection is not sufficient.

Required comparison methods:

* binary comparison
* compare_files()
* deterministic test vectors

---

## LAW 5 — Observable State

Crypto tests must expose internal states during debugging.

Examples:

* plaintext
* ciphertext
* IV
* key
* padding bytes
* packed blocks

All binary state logs must use hexadecimal formatting.

---

## LAW 6 — Minimal Failure Scope

Debugging must begin from the smallest failing case.

Recommended progression:

```text
1 block
→ 2 blocks
→ repeated blocks
→ large files
```

---

## LAW 7 — PASS/FAIL Clarity

Every test must terminate with exactly one of:

```text
STATUS: PASS
```

or

```text
STATUS: FAIL
```

No alternative wording is permitted.

---

# 4. Test Directory Structure

```text
tests/
│
├── unit/
│   ├── xtea/
│   ├── packing/
│   ├── padding/
│   ├── sha256/
│   ├── hmac/
│   └── pbkdf2/
│
├── integration/
│   ├── cbc_pipeline/
│   ├── file_roundtrip/
│   └── multi_block/
│
├── security/
│   ├── tamper/
│   ├── wrong_key/
│   ├── malformed/
│   └── corruption/
│
├── stress/
│   ├── large_files/
│   └── random_data/
│
├── test_vectors/
│
├── test_utils/
│
└── TEST_PLAN.md
```

---

# 5. Test Naming Rules

## Source File Naming

Pattern:

```text
<feature>_<purpose>_test.c
```

Examples:

```text
xtea_roundtrip_test.c
packing_roundtrip_test.c
cbc_padding_integration_test.c
wrong_key_security_test.c
```

---

## Test ID Naming

Pattern:

```text
<TYPE>-<MODULE>-<NUMBER>
```

Examples:

```text
UNIT-XTEA-01
INT-CBC-03
SEC-TAMPER-02
STR-LARGEFILE-01
```

---

# 6. Test Categories

---

# 6.1 Unit Tests

## Purpose

Validation of isolated components.

## Characteristics

* deterministic
* isolated
* fast
* small
* reproducible

## Examples

* XTEA encrypt/decrypt
* packing/unpacking
* PKCS#7 validation
* SHA256 vectors
* HMAC vectors

---

# 6.2 Integration Tests

## Purpose

Validation of interaction between modules.

## Examples

* CBC + XTEA
* processor + fileio
* encryption pipeline
* padding + decryption

---

# 6.3 Security Tests

## Purpose

Validation against invalid or malicious states.

## Examples

* wrong key
* corrupted ciphertext
* modified IV
* malformed padding
* tampered headers

---

# 6.4 Stress Tests

## Purpose

Validation of stability under scale and repetition.

## Examples

* large files
* repeated execution
* random binary data
* long-duration execution

---

# 7. Standard Test Structure

All tests must follow:

```text
Arrange
Act
Assert
Result
```

---

# 7.1 Arrange

Prepare:

* inputs
* buffers
* files
* IVs
* keys
* expected outputs

---

# 7.2 Act

Execute target operation.

Examples:

```text
encrypt
decrypt
hash
derive key
```

---

# 7.3 Assert

Validate exact correctness.

Examples:

```text
expected == observed
```

---

# 7.4 Result

Allowed outputs:

```text
STATUS: PASS
STATUS: FAIL
```

---

# 8. Standard Logging Format

All tests must use the following output structure.

---

# 8.1 Header

```text
========================================
TEST ID   : INT-CBC-01
TEST NAME : CBC Roundtrip Integration
========================================
```

---

# 8.2 Purpose

```text
[Purpose]

Validate CBC encryption and decryption roundtrip.
```

---

# 8.3 Input Section

```text
[Input]

Plaintext:
31 32 33 34 35 36 37 38

Key:
11 11 11 11 ...

IV:
AA BB CC DD ...
```

---

# 8.4 Action Section

```text
[Action]

1. Encrypt plaintext
2. Decrypt ciphertext
3. Compare restored plaintext
```

---

# 8.5 Observed Section

```text
[Observed]

Ciphertext:
7E 41 D5 50 ...

Restored:
31 32 33 34 ...
```

---

# 8.6 Validation Section

```text
[Validation]

Expected:
restored plaintext == original plaintext

Observed:
MATCH
```

---

# 8.7 Final Result

Only the following outputs are permitted:

```text
STATUS: PASS
```

or

```text
STATUS: FAIL
```

---

# 9. Hex Formatting Rules

All binary values must use:

```text
uppercase hexadecimal
```

Example:

```text
AA BB CC DD
```

Lowercase hexadecimal must not be used in logs.

---

# 10. Comparison Rules

Binary comparisons must use:

```text
byte-for-byte comparison
```

The following are prohibited:

* visual inspection
* string comparison
* partial matching

---

# 11. Debugging Rules

Temporary debugging logs must:

* remain removable
* not alter crypto logic
* not modify timing-sensitive operations
* not affect ciphertext generation

---

# 12. Security Testing Rules

Security tests must intentionally introduce:

* corrupted ciphertext
* modified IVs
* malformed padding
* wrong keys
* tampered headers

Expected failures must still produce deterministic logs.

---

# 13. Regression Testing Rules

When a bug is fixed:

* create dedicated regression test
* preserve failing input
* preserve expected output
* preserve reproduction steps

Regression tests must not be deleted.

---

# 14. Shared Test Utilities

Required shared utilities inside:

```text
tests/test_utils/
```

Required helpers:

```text
compare_files()
hex_dump()
print_block()
generate_test_file()
```

---

# 15. Required Initial Test Coverage

---

# 15.1 Unit Coverage

Required:

* XTEA roundtrip
* packing roundtrip
* PKCS#7 validation

---

# 15.2 Integration Coverage

Required:

* CBC roundtrip
* multiblock encryption
* repeated plaintext blocks
* exact-block PKCS#7 edge case

---

# 15.3 Security Coverage

Required:

* wrong IV
* wrong key
* ciphertext tampering
* malformed padding

---

# 16. Final Principle

The purpose of testing is not:

```text
make the application appear functional
```

The purpose of testing is:

```text
build measurable confidence in correctness
```

Testing infrastructure is considered part of the cryptographic system itself.
