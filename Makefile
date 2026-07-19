# *****************************************************************************
# *                                                                           *
# *                                                               ::::::::::  *
# *  Makefile                                                    :+:+:+:+:+   *
# *                                                                 +:+       *
# *  By: Jack (Khalid Oumouh)                                      +#+        *
# *                                                               +#+         *
# *  Created: 2026/07/05 13:26 by Jack                    #+#    #+#          *
# *  Updated: 2026/07/13 02:18 by Jack                     ########           *
# *                                                                           *
# *****************************************************************************




CC  ?= clang
TARGET_TRIPLE := i686-elf
BUILD_DIR := build
KERNEL := $(BUILD_DIR)/naoixos.elf

SRC_C := $(wildcard kernel/*.c drivers/*.c shell/*.c lib/*.c arch/*.c)
SRC_S := boot/boot.S boot/interrupts.S
OBJ   := $(SRC_C:%.c=$(BUILD_DIR)/%.o) $(SRC_S:%.S=$(BUILD_DIR)/%.o)

CC_VERSION := $(shell $(CC) --version 2>/dev/null)
ifneq (,$(findstring clang,$(CC_VERSION)))
    COMPILER_ID := clang
else
    COMPILER_ID := gcc
endif

CFLAGS  := -std=gnu11 -ffreestanding -fno-pic -fno-pie -fno-stack-protector \
           -mgeneral-regs-only \
           -Wall -Wextra -Iinclude -O2
ASFLAGS :=

ifeq ($(COMPILER_ID),clang)
    CFLAGS  += --target=$(TARGET_TRIPLE)
    ASFLAGS += --target=$(TARGET_TRIPLE)
    LD      := ld.lld
    LDFLAGS := -m elf_i386 -Wl,--strip-all
else
    CFLAGS  += -m32
    ASFLAGS += -m32
    LD      := $(CC)
    LDFLAGS := -m32 -ffreestanding -nostdlib -static -Wl,--strip-all
endif

LEGACY_BUILD_DIR := build-legacy
LEGACY_SRC_C := $(wildcard kernel/*.c drivers/*.c shell/*.c lib/*.c arch/*.c)
LEGACY_SRC_S := boot/interrupts.S boot/legacy_entry.S
LEGACY_OBJ   := $(LEGACY_SRC_C:%.c=$(LEGACY_BUILD_DIR)/%.o) $(LEGACY_SRC_S:%.S=$(LEGACY_BUILD_DIR)/%.o)

OBJCOPY := $(shell command -v llvm-objcopy 2>/dev/null || command -v objcopy 2>/dev/null)

.PHONY: all iso run run-grub img run-img clean

# build kernel

all: $(KERNEL)
	@echo "[naoixos] built with $(COMPILER_ID)"

$(KERNEL): $(OBJ) linker.ld
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $(OBJ)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

# run in qemu

run: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL) -m 128 -nographic

# build iso, grub tools needed

iso: $(KERNEL)
	mkdir -p $(BUILD_DIR)/isodir/boot/grub
	cp $(KERNEL) $(BUILD_DIR)/isodir/boot/naoixos.elf
	cp grub.cfg $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/naoixos.iso $(BUILD_DIR)/isodir

# run ISO in qemu

run-grub: iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/naoixos.iso -m 128 -nographic

$(LEGACY_BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LEGACY_BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

$(LEGACY_BUILD_DIR)/kernel.elf: $(LEGACY_OBJ) linker-legacy.ld
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -T linker-legacy.ld -o $@ $(LEGACY_OBJ)

$(LEGACY_BUILD_DIR)/kernel.bin: $(LEGACY_BUILD_DIR)/kernel.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/stage1.o: boot/stage1.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/stage1.elf: $(BUILD_DIR)/stage1.o boot/stage1.ld
	$(LD) $(LDFLAGS) -T boot/stage1.ld -o $@ $(BUILD_DIR)/stage1.o

$(BUILD_DIR)/stage1.bin: $(BUILD_DIR)/stage1.elf
	$(OBJCOPY) -O binary $< $@

# build floppy img

img: $(BUILD_DIR)/stage1.bin $(LEGACY_BUILD_DIR)/kernel.bin
	cat $(BUILD_DIR)/stage1.bin $(LEGACY_BUILD_DIR)/kernel.bin > $(BUILD_DIR)/naoixos-floppy.img
	truncate -s 1474560 $(BUILD_DIR)/naoixos-floppy.img
	@echo "[naoixos] wrote $(BUILD_DIR)/naoixos-floppy.img (mount as Floppy in Limbo)"

# run floppy img in qemu

run-img: img
	qemu-system-i386 -fda $(BUILD_DIR)/naoixos-floppy.img -m 128 -nographic

# clean from build files

clean:
	rm -rf $(BUILD_DIR) $(LEGACY_BUILD_DIR) */*.o
