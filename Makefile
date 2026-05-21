#Shell
SHELL= powershell.exe
#Compiler
CC = gcc

#Compiler Flags, 
#-Wpedantic: Enforces strict language standards.
#-g: Enables source code debugging.
#-static-libasan: Catches runtime memory errors.(ASan)
#-static-libubsan: Detects undefined code behaviors. (UBSan)
CFLAGS = -Wall -Wextra -Wpedantic -MMD -MP
SANITIZER_FLAGS = -static-libasan -static-libubsan -fsanitize=address -fsanitize=undefined
RELEASE_FLAGS = -O3
DEBUG_FLAGS = -O0 -g
#Vars
BUILD_BIN = build/bin
BUILD_OBJ = build/obj
SRC_DIR = src
TARGET = $(BUILD_BIN)/securecrypt.exe
INCLUDE = -I./include
#Source Files
SRC = $(wildcard $(SRC_DIR)/*.c)	
#Obj
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_OBJ)/%.o,$(SRC))

.PHONY:	all	build cleanb cleano clean

#Default Target
all : $(TARGET)

#Build rule
$(TARGET): $(OBJ)
	@if (-not (Test-Path $(BUILD_BIN))){New-Item -ItemType directory -Path $(BUILD_BIN) | Out-null}
	@Write-Host "[LINK] Linking executable..."
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) -o $(TARGET)
	@Write-Host "[SUCCESS] Linking complete`n[INFO] Executables written at $(TARGET)"


$(BUILD_OBJ)/%.o : $(SRC_DIR)/%.c
	@if (-not (Test-Path $(BUILD_OBJ))){New-Item -ItemType directory -Path $(BUILD_OBJ) | Out-null}
	@Write-Host "[BUILD] Compiling Source..."
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@Write-Host "[SUCCESS] Compilation complete`n[INFO] Object files written at $(BUILD_OBJ)"

run:
	cd $(BUILD_BIN)
	./$(TARGET)

debug : CFLAGS += $(DEBUG_FLAGS)
debug : all

release : CFLAGS += $(RELEASE_FLAGS) 
release : all

sanitize : CFLAGS += $(SANITIZER_FLAGS)
sanitize : all

clean : cleano cleanb

cleanb : 
	@Write-Host "[CLEAN] Cleaning $(BUILD_BIN) directory"
	@if (Test-Path $(BUILD_BIN)){Remove-Item -Recurse -Force $(BUILD_BIN) }
	@Write-Host "[CLEAN] Cleaned $(BUILD_BIN) directory"
cleano :
	@Write-Host "[CLEAN] Cleaning $(BUILD_OBJ) directory"
	@if (Test-Path $(BUILD_OBJ)){Remove-Item -Recurse -Force $(BUILD_OBJ) }
	@Write-Host "[CLEAN] Cleaned $(BUILD_OBJ) directory"