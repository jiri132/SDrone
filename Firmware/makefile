### === CONFIGURATION ===
# Toolchain setup
CC := xtensa-esp32-elf-gcc
AR := xtensa-esp32-elf-ar
RANLIB := xtensa-esp32-elf-ranlib
CFLAGS := -Wall -Wextra -std=c11 -Os
LDFLAGS :=

# Flashing configuration
PORT ?= COM5
BAUD ?= 460800
BIN := firmware.elf

# Directory and file structure
SRC_DIRS := core hal\gpio hal\irq hal\spi hal\lora services\circularbuffer services\protocol services\hamming utils
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)\obj
MOD_DIR := $(BUILD_DIR)\modules

# List of modules to bundle
MODULES := core gpio irq spi lora circularbuffer protocol hamming utils
ARCHIVES := $(foreach mod,$(MODULES),$(MOD_DIR)/$(mod).a)

# Main file (outside structure)
MAIN_SRC := main.c
MAIN_OBJ := $(OBJ_DIR)/main.o

.PHONY: all clean flash

### === BUILD RULES ===

all: $(BIN)

# Final binary from all archives and main.o
$(BIN): $(ARCHIVES) $(MAIN_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Rule to archive all .o files in each folder into a .a archive
$(MOD_DIR)/%.a:
	@mkdir "$(MOD_DIR)" 2>NUL || exit 0
	$(AR) rcs $@ $(wildcard $(OBJ_DIR)\$*\*.o)
	$(RANLIB) $@

# Compile each .c file into a .o file (excluding main.c)
SRCS := \
	$(foreach dir,$(SRC_DIRS),$(wildcard $(dir)\*.c))
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

$(OBJ_DIR)/%.o : %.c
	@mkdir "$(dir $@)" 2>NUL || exit 0
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main.c separately
$(MAIN_OBJ): $(MAIN_SRC)
	@mkdir "$(dir $@)" 2>NUL || exit 0
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure .o files are built before archiving
$(ARCHIVES): $(OBJS)

### === FLASH TO DEVICE ===
flash: $(BIN)
	esptool.exe --chip esp32 --port $(PORT) --baud $(BAUD) write_flash 0x10000 $(BIN)

### === CLEANUP ===
clean:
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	del /q *.elf *.bin *.o *.a 2>NUL

### === NOTES ===
# .o files are compiled source files (machine code units).
# .a files are archives (libraries) that bundle multiple .o files together
# and include an index for the linker to resolve symbols efficiently.
# main.c is compiled standalone and not part of the archive system.