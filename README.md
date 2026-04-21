PROJECT TITLE:
Secure File Encryption and Decryption Tool using XTEA Algorithm in C


============================================================
1. PROJECT OVERVIEW
============================================================

This project is a file encryption and decryption tool developed in C using the XTEA (Extended Tiny Encryption Algorithm) cipher.

The program allows users to:

1. Encrypt any file
2. Decrypt encrypted files
3. Use a secret password/key
4. Protect data confidentiality
5. Demonstrate cryptography concepts using pure C implementation

XTEA is chosen because it is lightweight, secure, and easy to implement from scratch compared to other algorithms like AES.


============================================================
2. PROJECT ARCHITECTURE
============================================================

                    +-------------------+
                    |      User Menu    |
                    | 1.Encrypt File    |
                    | 2.Decrypt File    |
                    | 3.Exit            |
                    +---------+---------+
                              |
                              v
                    +-------------------+
                    |   Input Module    |
                    | File Name         |
                    | Output Name       |
                    | Password / Key    |
                    +---------+---------+
                              |
                              v
                    +-------------------+
                    | Key Generator     |
                    | Convert Password  |
                    | into 128-bit Key  |
                    +---------+---------+
                              |
                              v
                    +-------------------+
                    | File Handler      |
                    | Read File in      |
                    | 8-byte Blocks     |
                    +---------+---------+
                              |
                              v
                    +-------------------+
                    | XTEA Engine       |
                    | Encrypt / Decrypt |
                    | 32 Rounds Cipher  |
                    +---------+---------+
                              |
                              v
                    +-------------------+
                    | Output Module     |
                    | Save Encrypted or |
                    | Decrypted File    |
                    +-------------------+


============================================================
3. MODULE DESCRIPTION
============================================================

A. Main Module
--------------
Controls menu system and user interaction.

B. Input Module
---------------
Accepts:
- Source file name
- Destination file name
- Password

C. Key Generation Module
------------------------
Converts user password into 128-bit key
(4 unsigned integers for XTEA)

D. File Handling Module
-----------------------
- Opens files
- Reads data block-by-block
- Writes processed output

E. XTEA Cipher Module
---------------------
Performs:
- Encryption
- Decryption

Uses:
- 64-bit block size
- 128-bit key
- 32 rounds

F. Output Module
----------------
Creates:
- .enc encrypted file
- Restored decrypted file


============================================================
4. FOLDER STRUCTURE
============================================================

XTEA_File_Encryptor/
│
├── main.c
├── xtea.c
├── xtea.h
├── fileio.c
├── fileio.h
├── keygen.c
├── keygen.h
├── sample.txt
├── encrypted.enc
├── decrypted.txt
├── README.txt
└── report.docx


============================================================
5. FILE PURPOSE
============================================================

main.c
------
Program entry point.
Contains menu and function calls.

xtea.c
------
Contains XTEA encryption/decryption logic.

xtea.h
------
Header file for XTEA functions.

fileio.c
--------
Handles file reading/writing operations.

fileio.h
--------
Header for file functions.

keygen.c
--------
Converts password into 128-bit key.

keygen.h
--------
Header for key generation.

sample.txt
----------
Original input file.

encrypted.enc
-------------
Encrypted output file.

decrypted.txt
-------------
Recovered original file.


============================================================
6. PROJECT WORKFLOW
============================================================

STEP 1:
Run Program

STEP 2:
Display Menu

1. Encrypt File
2. Decrypt File
3. Exit

STEP 3:
User Selects Option

If Encrypt:
-----------
- Enter source file
- Enter output file
- Enter password

STEP 4:
Password converted into XTEA key

STEP 5:
Program reads file in 8-byte blocks

STEP 6:
Each block encrypted using XTEA

STEP 7:
Encrypted data written to output file

STEP 8:
Success message shown


If Decrypt:
-----------
- Enter encrypted file
- Enter output file
- Enter same password

STEP 9:
Program reads encrypted blocks

STEP 10:
Each block decrypted using XTEA

STEP 11:
Original file restored


============================================================
7. DATA FLOW
============================================================

Original File
     |
     v
Read 8 Bytes
     |
     v
Generate Key from Password
     |
     v
XTEA Encrypt
     |
     v
Encrypted File


Encrypted File
     |
     v
Read 8 Bytes
     |
     v
Generate Same Key
     |
     v
XTEA Decrypt
     |
     v
Original File


============================================================
8. SECURITY FEATURES
============================================================

1. Uses real block cipher (XTEA)
2. Password protected encryption
3. No plain text storage
4. Same password required for decryption
5. Multiple rounds increase security


============================================================
9. FUTURE ENHANCEMENTS
============================================================

1. CBC Mode
2. File Integrity Check
3. GUI Interface
4. Drag & Drop Files
5. Folder Encryption
6. Password Hiding
7. Stronger Key Derivation


============================================================
10. CONCLUSION
============================================================

This project demonstrates practical file security using cryptography in C.
The XTEA algorithm offers a strong balance between simplicity and security,
making it ideal for academic implementation from scratch.