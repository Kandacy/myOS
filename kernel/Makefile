
CROSS_PREFIX = riscv64-unknown-elf-
CC = ${CROSS_PREFIX}gcc
LD = ${CROSS_PREFIX}ld

OBJCOPY = riscv64-unknown-elf-objcopy
OBJDUMP = riscv64-unknown-elf-objdump

BASE_ADDRESS = 0x80200000

INC_DIR = ../include
SRC_LINKER = ./qemu.ld
TARGET_NAME = ../target/main
TARGET_ELF = ${TARGET_NAME}.elf
TARGET_BIN = ${TARGET_NAME}.bin

SRC_FILES = ./start.s \
			./lib/alloc.c \
			./lib/printk.c \
			./lib/error.c \
			./lib/string.c \
			./lib/vector.c \
			./mm/addr.c \
			./mm/frame_allocator.c \
			./mm/mem.c \
			./mm/page_table.c \
			./sbi/sbi.c \
			./syscall/syscall.c \
			./syscall/process.c \
			./task/switch.s \
			./task/task.c \
			./trap/trap.s \
			./trap/trap.c \
			./link_app.s \
			./loader.c \
			./main.c \
			./timer.c 

build: 
	@${CC} ${SRC_FILES} -o ${TARGET_ELF} \
		-T${SRC_LINKER} \
		-I${INC_DIR} \
		-nostdinc -nostdlib -mcmodel=medany -fno-builtin

dump:
	@${OBJDUMP} -h ${TARGET_ELF}

objcopy: build
	@${OBJCOPY} -S ${TARGET_ELF} -O binary ${TARGET_BIN}

# run with gdb
rung: objcopy
	@qemu-system-riscv64 \
		-machine virt \
		-nographic \
		-bios ../bootloader/rustsbi-qemu.bin \
		-device loader,file=${TARGET_NAME}.bin,addr=${BASE_ADDRESS} -s -S

run: objcopy
	@qemu-system-riscv64 \
		-machine virt \
		-nographic \
		-bios ../bootloader/rustsbi-qemu.bin \
		-device loader,file=${TARGET_NAME}.bin,addr=${BASE_ADDRESS}
#-append nokaslr	-kernel ${TARGET_NAME}.bin
#-smp 1


gdb: objcopy
	riscv-nuclei-elf-gdb -ex "set arch riscv:rv64" -ex "file ${TARGET_ELF}" -ex "target remote localhost:1234"


# // 执行命令
# // rust-objcopy --strip-all target/riscv64gc-unknown-none-elf/release/os -O binary target/riscv64gc-unknown-none-elf/release/os.bin
# // qemu-system-riscv64 -machine virt -nographic -bios ../bootloader/rustsbi-qemu.bin -device loader,file=target/riscv64gc-unknown-none-elf/release/os.bin,addr=0x80200000 -s -S
# // riscv-nuclei-elf-gdb
# // riscv-nuclei-elf-gdb -ex 'file target/riscv64gc-unknown-none-elf/release/os' -ex 'set arch riscv:rv64' -ex 'target remote localhost:1234'
# // riscv64-unknown-elf-gdb


