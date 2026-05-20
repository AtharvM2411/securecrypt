#Shell
SHELL= powershell.exe
#Compiler
CC = gcc

#Compiler Flags, 
#-Wpedantic: Enforces strict language standards.
#-g: Enables source code debugging.
#-fsanitize=address: Catches runtime memory errors.(ASan)
#-fsanitize=undefined: Detects undefined code behaviors. (UBSan)
CFLAGS = -Wall -Wextra -Wpedantic -static-libasan -static-libubsan -O3 -g

#Vars
BUILD = build
TARGET = $(BUILD)/securecrypt.exe
INCLUDE = -I./include
#Source Files
SRC = $(wildcard src/*.c)	

.PHONY:	all	clean

#Default Target
all : $(TARGET)

#Build rule
$(TARGET) : $(SRC)
	@if (-not (Test-Path $(BUILD))){New-Item -ItemType directory -Path $(BUILD) | Out-null}
	@Write-Host "[BUILD] Compiling Source to Target"
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o $(TARGET)
	@Write-Host "[SUCCESS] Compilation complete`n[INFO] Executables written at $(TARGET)"
run:
	cd $(BUILD)
	./$(TARGET)
clean : 
	@Write-Host "[CLEAN] Cleaning build directory"
	@if (Test-Path $(BUILD)){Remove-Item -Recurse -Force $(BUILD) }