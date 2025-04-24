# Toolchain & tools
CC = xtensa-esp32-elf-gcc
ESPTOOL = esptool.exe

# Port config
PORT ?= COM7
BAUD ?= 460800

# Source structure
SRC_DIRS = core hal/gpio hal/irq hal/spi hal/sx1278 services/circularbuffer services/hamming services/protocol utils
BUILD_DIR = build

# Flags
CFLAGS = -Os -Wall -std=c11 -ffunction-sections -fdata-sections -mlongcalls -nostdlib -Wno-unused-parameter -T linker.ld
LDFLAGS = -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group
INCLUDES = $(foreach dir,$(SRC_DIRS),-I$(dir))

# Targets
TARGET = firmware.elf
IMAGE = firmware-0x10000.bin

# Source and object files
SRC = $(wildcard $(addsuffix /*.c, $(SRC_DIRS))) main.c startup.c
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

# Default build
all: $(IMAGE)

# ELF
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(CFLAGS) $(LDFLAGS)

# BIN
$(IMAGE): $(TARGET)
	$(ESPTOOL) --chip esp32 elf2image -o $(IMAGE) $(TARGET)

# Compile objects
$(BUILD_DIR)/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Upload just firmware
upload: $(IMAGE)
	$(ESPTOOL) --chip esp32 --port $(PORT) --baud $(BAUD) write_flash -z 0x10000 $(IMAGE)

# Upload with full layout (bootloader + partition + app)
upload-all: $(IMAGE)
	$(ESPTOOL) --chip esp32 --port $(PORT) --baud $(BAUD) write_flash -z \
		0x1000 bootloader.bin \
		0x8000 partition_table.bin \
		0x10000 $(IMAGE)

# Clean
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@del /q $(TARGET) firmware*.bin 2>nul
