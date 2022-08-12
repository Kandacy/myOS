
CROSS_PREFIX = riscv64-unknown-elf-
CC = ${CROSS_PREFIX}gcc

OBJCOPY = rust-objcopy
OBJDUMP = rust-objdump

BASE_ADDRESS = 0x80200000

INC_DIR = ../include
SRC_LINKER = ./qemu.ld
TARGET_NAME = ../target/main

SRC_FILES = ./start.s \
			./main.c \
			./sbi/sbi.c \
			./lib/printk.c \
			./lib/error.c

build: 
	${CC} ${SRC_FILES} -o ${TARGET_NAME} -T${SRC_LINKER} -nostdinc -nostdlib -I${INC_DIR} -mcmodel=medany

dump:
	${OBJDUMP} -h ${TARGET_NAME}

run: build
	${OBJCOPY} --strip-all ${TARGET_NAME} -O binary ${TARGET_NAME}.bin
	qemu-system-riscv64 -machine virt -nographic -bios ../bootloader/rustsbi-qemu.bin -device loader,file=${TARGET_NAME}.bin,addr=${BASE_ADDRESS} 

