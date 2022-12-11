
CROSS_PREFIX = riscv64-unknown-elf-
CC = ${CROSS_PREFIX}gcc

OBJCOPY = rust-objcopy
OBJDUMP = rust-objdump

INC_DIR = ./include
APP_NUM = 02
# SRC_LINKER = ./linker/${APP_NUM}t_linker.ld
SRC_LINKER = ./linker.ld
TARGET_NAME = ./target/${APP_NUM}t
TARGET_ELF = ${TARGET_NAME}.elf
TARGET_BIN = ${TARGET_NAME}.bin

SRC_FILES = ./start.s \
			./main.c \
			./syscall.c \
			./lib/printf.c \
			./lib/string.c \
			./app/${APP_NUM}t.c

build: 
	@${CC} ${SRC_FILES} -o ${TARGET_ELF} \
		-T${SRC_LINKER} \
		-I${INC_DIR} \
		-nostdinc -nostdlib -mcmodel=medany -fno-builtin
	@${OBJCOPY} -S ${TARGET_ELF} -O binary ${TARGET_BIN}


dump:
	${OBJDUMP} -h ${TARGET_ELF}


clean:
	rm ./target/*



