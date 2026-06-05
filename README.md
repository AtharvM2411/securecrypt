# SecureCrypt

### Educational File Encryption System in C

![Language](https://img.shields.io/badge/C-C11-blue)
![Build](https://img.shields.io/badge/Build-Make-success)
![Architecture](https://img.shields.io/badge/Architecture-Modular-success)
![Testing](https://img.shields.io/badge/Testing-Unit%20%26%20Integration-green)
![Cipher](https://img.shields.io/badge/Cipher-XTEA-red)
![Mode](https://img.shields.io/badge/Mode-CBC-purple)
![License](https://img.shields.io/badge/License-MIT-yellow)

SecureCrypt is an educational cryptography project that demonstrates how a modern file encryption pipeline can be built from low-level components in C.

The project implements:

- XTEA block cipher
- CBC (Cipher Block Chaining) mode
- PKCS#7 padding
- Modular file processing
- Automated build system
- Unit and integration testing

The codebase is intentionally structured for learning, experimentation, and discussion rather than production deployment.

## Design Goals

This project was built to explore:

- Block cipher implementation
- Cipher mode construction
- Data packing and serialization
- Binary file processing
- Modular software architecture
- Build automation with Make
- Cryptographic testing methodologies

## Security Notice

SecureCrypt is an educational implementation.

While it successfully demonstrates encryption and decryption workflows, several security features commonly required in production-grade encryption software are still under development, including:

- PBKDF2 key derivation
- HMAC authentication
- Random IV generation
- File format versioning
- Tamper detection

### This project should be used for educational purposes only.

# Architecture

SecureCrypt follows a layered architecture that separates cryptographic primitives, file processing, and application logic.

```mermaid
flowchart TD

    main[main.c]

    processor[processor.c]
    fileio[fileio.c]

    cbc[cbc.c]
    padding[padding.c]
    xtea[xtea.c]

    main --> processor

    processor --> fileio
    processor --> padding
    processor --> cbc
    processor --> xtea
```

This separation improves maintainability, testability, and allows individual components to evolve independently.



# Design Goals

The project was built to explore:

* Cryptographic system design
* Block cipher implementation
* Cipher mode construction
* Binary file processing
* Modular software architecture
* Build automation
* Software testing practices

The primary objective is educational value and engineering clarity rather than production deployment.



# Cryptographic Pipeline

Encryption follows a staged processing pipeline:

```mermaid
flowchart LR

    A[Input File]
    B[Read Chunk]
    C[PKCS#7 Padding]
    D[CBC XOR]
    E[XTEA Encrypt]
    F[Write Ciphertext]

    A --> B
    B --> C
    C --> D
    D --> E
    E --> F
```

Decryption performs the reverse sequence to recover the original plaintext.
```mermaid
flowchart LR

    A[Ciphertext File]
    B[Read Chunk]
    C[XTEA Decrypt]
    D[CBC Reverse XOR]
    E[Remove Padding]
    F[Recovered File]

    A --> B
    B --> C
    C --> D
    D --> E
    E --> F
```

# Testing Philosophy

Testing is integrated throughout the project development process.

The codebase includes:

* Unit tests for individual components
* Integration tests for end-to-end workflows
* Infrastructure for future security testing
* Infrastructure for future stress testing

Testing focuses on correctness, reversibility, data integrity, and component isolation.


# Build System

The project uses a Makefile-driven build process.

Key goals include:

* Reproducible builds
* Automated dependency tracking
* Separation of source and build artifacts
* Scalable project organization

Build artifacts are generated in dedicated build directories to keep source code clean and organized.


# Engineering Focus Areas

This project explores several software engineering topics:

* Modular design in C
* Memory-safe data processing
* Cryptographic primitives
* Binary serialization
* File handling
* Automated builds
* Testing methodologies
* Debugging low-level systems software


# Roadmap

Future development may include:

* Password-based key derivation
* Cryptographic authentication
* Secure metadata handling
* Randomized initialization vectors
* File format versioning
* Tamper detection mechanisms
* Expanded test coverage
* Performance profiling and optimization

```mermaid
flowchart TD

    User[User Password]

    PBKDF2[PBKDF2]
    SHA256[SHA-256]
    HMAC[HMAC]
    IV[Random IV]

    CBC[CBC Mode]
    XTEA[XTEA]

    User --> PBKDF2
    PBKDF2 --> XTEA

    IV --> CBC
    CBC --> XTEA

    XTEA --> HMAC
    SHA256 --> HMAC
```

# Author

Built and maintained as an ongoing systems programming and cryptography project.

The project serves as a practical exploration of encryption system design, software architecture, testing methodologies, and low-level development in C.

Contributions, discussions, and feedback are welcome.
~AtharvM2411