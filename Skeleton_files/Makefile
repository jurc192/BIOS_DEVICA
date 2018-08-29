#
# Default makefile
#
# Assembles a binary file using: "make" or "make all"
# Creates a raw disk image using: "make image"
# Runs a qemu-vm using "make run"

SHELL := /bin/bash
FILENAME=default

SRC_DIR=src
BUILD_DIR=builds

# assemble source file and make floppy image file in builds/ folder
$(BUILD_DIR)/$(FILENAME): $(SRC_DIR)/$(FILENAME).asm
	nasm -f bin -o $(BUILD_DIR)/$(FILENAME) $(SRC_DIR)/$(FILENAME).asm
	dd if=/dev/zero of=$(BUILD_DIR)/$(FILENAME).img bs=1024 count=720
	dd if=$(BUILD_DIR)/$(FILENAME) of=$(BUILD_DIR)/$(FILENAME).img conv=notrunc
	printf '\x55\xAA' | dd of=$(BUILD_DIR)/$(FILENAME).img bs=1 seek=510 count=2 conv=notrunc

$(BUILD_DIR)/$(FILENAME).img: $(BUILD_DIR)/$(FILENAME)

# run image file w/ qemu
run: $(BUILD_DIR)/$(FILENAME).img
	qemu-system-x86_64 -hda $(BUILD_DIR)/$(FILENAME).img

clean:
	rm -f $(BUILD_DIR)/$(FILENAME) $(BUILD_DIR)/$(FILENAME).img

.PHONY: run clean