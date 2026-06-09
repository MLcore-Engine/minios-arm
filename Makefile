TARGET := minios-arm
BUILD_DIR := build

LLVM_PREFIX ?= $(shell brew --prefix llvm 2>/dev/null)
LLD_PREFIX ?= $(shell brew --prefix lld 2>/dev/null)
LLVM_BIN := $(if $(LLVM_PREFIX),$(LLVM_PREFIX)/bin/,)
LLD_BIN := $(if $(LLD_PREFIX),$(LLD_PREFIX)/bin/,)

CC := $(LLVM_BIN)clang
LD := $(LLD_BIN)ld.lld
OBJCOPY := $(LLVM_BIN)llvm-objcopy
QEMU := qemu-system-aarch64

CFLAGS := --target=aarch64-elf -Wall -Wextra -O2 -ffreestanding -nostdlib -nostdinc
CFLAGS += -mgeneral-regs-only -fno-builtin -fno-stack-protector
CFLAGS += -Isrc/include

LDFLAGS := -T linker.ld -nostdlib

SRCS := \
	src/boot.S \
	src/kernel/main.c \
	src/drivers/uart.c

OBJS := $(patsubst src/%,$(BUILD_DIR)/%.o,$(SRCS))
ELF := $(BUILD_DIR)/$(TARGET).elf
IMG := $(BUILD_DIR)/kernel8.img

.PHONY: all clean run debug

all: $(ELF) $(IMG)

$(BUILD_DIR)/%.S.o: src/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(IMG): $(ELF)
	$(OBJCOPY) -O binary $< $@

run: $(ELF)
	$(QEMU) -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(ELF)

debug: $(ELF)
	$(QEMU) -M virt -cpu cortex-a53 -nographic -serial mon:stdio -kernel $(ELF) -S -s

clean:
	rm -rf $(BUILD_DIR)
