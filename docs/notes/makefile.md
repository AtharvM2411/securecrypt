# Makefile Build System Documentation

For the SecureCrypt File Encryptor project

This Makefile implements a modular and scalable build system for the SecureCrypt encryption tool. It automates:

* source discovery
* object generation
* executable linking
* debug builds
* release builds
* sanitizer-enabled builds
* cleanup operations
* dependency tracking

The build system is designed for:

* maintainability
* modular compilation
* faster incremental builds
* safer debugging workflows
* scalable project growth

---

# 1. Build System Architecture

```text id="wjglmbm"
Source Files (.c)
        │
        ▼
Object Files (.o)
        │
        ▼
Executable (.exe)
```

The Makefile separates the build pipeline into:

| Stage               | Purpose                            |
| ------------------- | ---------------------------------- |
| Compilation         | Converts `.c` → `.o`               |
| Linking             | Combines `.o` → executable         |
| Dependency Tracking | Detects header changes             |
| Build Configuration | Debug / Release / Sanitized builds |

---

# 2. Project Directory Structure

```text id="wjglmbn"
project/
│
├── src/
│   ├── main.c
│   ├── xtea.c
│   ├── cbc.c
│   ├── padding.c
│   └── ...
│
├── include/
│   ├── xtea.h
│   ├── cbc.h
│   └── ...
│
├── build/
│   ├── obj/
│   │   ├── *.o
│   │   └── *.d
│   │
│   └── bin/
│       └── securecrypt.exe
│
└── Makefile
```

---

# 3. Shell Configuration

```make id="wjglmbo"
SHELL = powershell.exe
```

The build system uses PowerShell as the command execution shell.

This enables:

* `Write-Host`
* `Test-Path`
* `New-Item`
* `Remove-Item`

for build logging and directory management on Windows.

---

# 4. Compiler Configuration

```make id="wjglmbp"
CC = gcc
```

Defines the compiler executable used throughout the build system.

The architecture is compiler-agnostic and can later be switched to:

* Clang
* cross-compilers
* custom GCC toolchains

by modifying only this variable.

---

# 5. Compiler Flags

## Base Flags

```make id="wjglmbq"
CFLAGS = -Wall -Wextra -Wpedantic -MMD -MP
```

### Purpose

| Flag         | Description                                     |
| ------------ | ----------------------------------------------- |
| `-Wall`      | Enables common warnings                         |
| `-Wextra`    | Enables additional warnings                     |
| `-Wpedantic` | Enforces strict ISO C compliance                |
| `-MMD`       | Generates automatic dependency files            |
| `-MP`        | Adds dummy dependency rules for removed headers |

---

# 6. Dependency Tracking System

The Makefile uses automatic dependency tracking.

During compilation:

```text id="wjglmbr"
xtea.c
```

generates:

```text id="wjglmbs"
xtea.o
xtea.d
```

The `.d` files contain header dependency information.

Example:

```make id="wjglmbt"
build/obj/xtea.o: src/xtea.c include/xtea.h
```

This ensures:

* header changes trigger rebuilds
* stale object files are avoided
* incremental builds remain correct

---

# 7. Build Configurations

The build system supports multiple compilation modes.

---

## Debug Build

```make id="wjglmbu"
DEBUG_FLAGS = -O0 -g
```

### Characteristics

| Feature       | Value                   |
| ------------- | ----------------------- |
| Optimization  | Disabled                |
| Debug Symbols | Enabled                 |
| Intended Use  | Development & debugging |

### Purpose

Improves:

* GDB debugging
* readable stack traces
* source-level inspection

---

## Release Build

```make id="wjglmbv"
RELEASE_FLAGS = -O3
```

### Characteristics

| Feature       | Value             |
| ------------- | ----------------- |
| Optimization  | Maximum           |
| Debug Symbols | Disabled          |
| Intended Use  | Production builds |

### Purpose

Improves:

* runtime performance
* encryption throughput
* binary efficiency

---

## Sanitizer Build

```make id="wjglmbw"
SANITIZER_FLAGS = -static-libasan -static-libubsan -fsanitize=address -fsanitize=undefined
```

### Enabled Sanitizers

| Sanitizer                          | Purpose                    |
| ---------------------------------- | -------------------------- |
| AddressSanitizer (ASan)            | Detects memory corruption  |
| UndefinedBehaviorSanitizer (UBSan) | Detects undefined behavior |

### Detectable Issues

* buffer overflows
* invalid frees
* use-after-free
* stack corruption
* undefined arithmetic behavior
* invalid pointer operations

### Intended Use

* debugging
* memory validation
* security testing

Sanitizer builds are NOT intended for production deployment.

---

# 8. Build Variables

```make id="wjglmbx"
BUILD_BIN = build/bin
BUILD_OBJ = build/obj
SRC_DIR = src
TARGET = $(BUILD_BIN)/securecrypt.exe
```

### Purpose

| Variable    | Description                              |
| ----------- | ---------------------------------------- |
| `BUILD_BIN` | Final executable output directory        |
| `BUILD_OBJ` | Intermediate object/dependency directory |
| `SRC_DIR`   | Source code directory                    |
| `TARGET`    | Final executable path                    |

---

# 9. Include Directory

```make id="wjglmby"
INCLUDE = -I./include
```

Adds:

```text id="wjglmbz"
./include
```

to the compiler header search path.

Allows clean includes such as:

```c id="wjglmc0"
#include "xtea.h"
```

instead of fragile relative paths.

---

# 10. Automatic Source Discovery

```make id="wjglmc1"
SRC = $(wildcard $(SRC_DIR)/*.c)
```

Automatically discovers all `.c` files inside:

```text id="wjglmc2"
src/
```

This eliminates manual source list maintenance.

Adding a new source file automatically integrates it into the build pipeline.

---

# 11. Object File Generation

```make id="wjglmc3"
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_OBJ)/%.o,$(SRC))
```

Transforms source filenames into object filenames.

Example:

```text id="wjglmc4"
src/xtea.c
↓
build/obj/xtea.o
```

This mapping is fundamental to the compilation pipeline.

---

# 12. PHONY Targets

```make id="wjglmc5"
.PHONY: all build cleanb cleano clean
```

Marks targets as non-file commands.

Ensures commands execute even if files with matching names exist.

---

# 13. Default Build Target

```make id="wjglmc6"
all : $(TARGET)
```

Defines the default build goal.

Running:

```bash id="wjglmc7"
mingw32-make
```

automatically builds:

```text id="wjglmc8"
securecrypt.exe
```

---

# 14. Linking Stage

```make id="wjglmc9"
$(TARGET): $(OBJ)
```

Defines executable dependencies.

The executable depends on ALL object files.

---

## Link Command

```make id="wjglmca"
$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) -o $(TARGET)
```

Combines:

```text id="wjglmcb"
.o files
```

into:

```text id="wjglmcc"
securecrypt.exe
```

This is the final linking stage.

---

# 15. Pattern Rule Compilation System

```make id="wjglmcd"
$(BUILD_OBJ)/%.o : $(SRC_DIR)/%.c
```

Defines a reusable pattern rule.

Example transformation:

```text id="wjglmce"
src/cbc.c
↓
build/obj/cbc.o
```

---

## Compilation Command

```make id="wjglmcf"
$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
```

### Automatic Variables

| Variable | Meaning           |
| -------- | ----------------- |
| `$<`     | Source dependency |
| `$@`     | Target output     |

---

## Example Expansion

```bash id="wjglmcg"
gcc ... -c src/xtea.c -o build/obj/xtea.o
```

---

# 16. Build Logging System

The Makefile includes structured build logging using:

```powershell id="wjglmch"
Write-Host
```

This improves:

* observability
* debugging clarity
* build readability

Example:

```text id="wjglmci"
[BUILD] Compiling Source...
[LINK] Linking executable...
[SUCCESS] Linking complete
```

---

# 17. Run Target

```make id="wjglmcj"
run:
```

Executes the compiled application.

Recommended simplified implementation:

```make id="wjglmck"
run:
	./$(TARGET)
```

---

# 18. Cleanup Targets

---

## Binary Cleanup

```make id="wjglmcl"
cleanb
```

Removes:

```text id="wjglmcm"
build/bin
```

---

## Object Cleanup

```make id="wjglmcn"
cleano
```

Removes:

```text id="wjglmco"
build/obj
```

including:

* `.o`
* `.d`

files.

---

## Full Cleanup

```make id="wjglmcp"
clean : cleano cleanb
```

Performs complete build artifact cleanup.

---

# 19. Build Commands

## Default Build

```bash id="wjglmcq"
mingw32-make
```

---

## Debug Build

```bash id="wjglmcr"
mingw32-make debug
```

---

## Release Build

```bash id="wjglmcs"
mingw32-make release
```

---

## Sanitized Build

```bash id="wjglmct"
mingw32-make sanitize
```

---

## Run Application

```bash id="wjglmcu"
mingw32-make run
```

---

## Clean Build Artifacts

```bash id="wjglmcv"
mingw32-make clean
```

---

# 20. Engineering Characteristics

This Makefile demonstrates several important software engineering concepts:

| Feature                  | Purpose                       |
| ------------------------ | ----------------------------- |
| Pattern Rules            | Scalable compilation          |
| Automatic Discovery      | Reduced maintenance           |
| Dependency Tracking      | Correct incremental builds    |
| Build Separation         | Clean artifact management     |
| Sanitizer Integration    | Runtime safety analysis       |
| Configurable Build Modes | Flexible development workflow |
| Structured Logging       | Improved observability        |

---

# 21. Future Enhancements

Potential future improvements include:

* separate debug/release directories
* automatic test runner integration
* benchmark targets
* static analysis integration
* CI/CD pipelines
* cross-platform compatibility
* auto-generated dependency includes
* unit test orchestration

---

# 22. Conclusion

This Makefile implements a scalable modular build system suitable for medium-sized systems programming projects in C.

The architecture supports:

* maintainable compilation
* scalable project growth
* safe debugging workflows
* automated dependency management
* professional build organization

It provides a strong engineering foundation for the ongoing development of the SecureCrypt encryption tool.
