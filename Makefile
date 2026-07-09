# ==============================================================================
# Toolchain & Architecture Setup
# ==============================================================================
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

MCU     = -mcpu=cortex-m4 -mthumb
CFLAGS  = $(MCU) -O0 -g -Wall -Iinc
LDFLAGS = -T linker.ld -nostdlib $(MCU)

# ==============================================================================
# Project Structure
# ==============================================================================
TARGET    = project
SRC_DIR   = src
BUILD_DIR = build

# Automatically find all .c files in src/ and map them to .o files in build/
SOURCES   = $(wildcard $(SRC_DIR)/*.c)
OBJECTS   = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# ==============================================================================
# Build Targets
# ==============================================================================
all: $(BUILD_DIR)/$(TARGET).bin size

# 3. Extract raw binary
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# 2. Link object files into ELF
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# 1. Compile C files into object files (Creates build dir if missing)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# Create the build directory
$(BUILD_DIR):
	mkdir -p $@

# ==============================================================================
# Utility Targets
# ==============================================================================
.PHONY: all clean flash size

size: $(BUILD_DIR)/$(TARGET).elf
	$(SIZE) $<

# Wipe the entire build directory
clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET).elf
	openocd -f board/st_nucleo_f4.cfg -c "program $< verify reset exit"
