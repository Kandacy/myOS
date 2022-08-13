
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
			./lib/error.c \
			./lib/string.c \
			./trap/trap.s \
			./trap/trap.c \
			./link_app.s \
			./loader.c \
			./syscall/syscall.c \
			./syscall/fs.c 

build: 
	${CC} ${SRC_FILES} -o ${TARGET_NAME} \
		-T${SRC_LINKER} \
		-I${INC_DIR} \
		-nostdinc -nostdlib -mcmodel=medany -fno-builtin

dump:
	${OBJDUMP} -h ${TARGET_NAME}

run: build
	${OBJCOPY} --strip-all ${TARGET_NAME} -O binary ${TARGET_NAME}.bin
	qemu-system-riscv64 \
		-machine virt \
		-nographic \
		-bios ../bootloader/rustsbi-qemu.bin \
		-device loader,file=${TARGET_NAME}.bin,addr=${BASE_ADDRESS} #-s -S



# // 执行命令
# // rust-objcopy --strip-all target/riscv64gc-unknown-none-elf/release/os -O binary target/riscv64gc-unknown-none-elf/release/os.bin
# // qemu-system-riscv64 -machine virt -nographic -bios ../bootloader/rustsbi-qemu.bin -device loader,file=target/riscv64gc-unknown-none-elf/release/os.bin,addr=0x80200000 -s -S
# // riscv-nuclei-elf-gdb
# // riscv-nuclei-elf-gdb -ex 'file target/riscv64gc-unknown-none-elf/release/os' -ex 'set arch riscv:rv64' -ex 'target remote localhost:1234'
# // riscv64-unknown-elf-gdb


