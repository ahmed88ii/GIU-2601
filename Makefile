# ============================================================
# Makefile - ATmega32 Embedded Systems Project
# ============================================================
# Usage:
#   make          - build main.hex
#   make flash    - flash to MCU via USBasp
#   make size     - show memory usage
#   make clean    - remove build artefacts
# ============================================================

MCU        := atmega32
F_CPU      := 8000000UL
CC         := avr-gcc
OBJCOPY    := avr-objcopy
AVRDUDE    := avrdude
PROGRAMMER := usbasp
PORT       := usb

TARGET    := main
BUILD_DIR := build
SRC_DIR   := src

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

CFLAGS := \
    -mmcu=$(MCU)        \
    -DF_CPU=$(F_CPU)    \
    -Os                 \
    -std=c99            \
    -Wall               \
    -Wextra             \
    -ffunction-sections \
    -fdata-sections     \
    -I$(SRC_DIR)

LDFLAGS := \
    -mmcu=$(MCU)        \
    -Wl,--gc-sections

.PHONY: all flash clean size

all: $(TARGET).hex

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "Build complete: $(TARGET).hex"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) \
	    -U flash:w:$(TARGET).hex:i

size: $(TARGET).elf
	avr-size --format=avr --mcu=$(MCU) $(TARGET).elf

clean:
	rm -rf $(BUILD_DIR) $(TARGET).elf $(TARGET).hex
