
CROSS_PREFIX = riscv64-unknown-elf-
CC = ${CROSS_PREFIX}gcc

OBJCOPY = rust-objcopy
OBJDUMP = rust-objdump

INC_DIR = ./include
SRC_LINKER = ./linker.ld
TARGET_NAME = ./target/main

SRC_FILES = ./start.s \
			./main.c \
			./syscall.c \
			./lib/printf.c \
			./lib/string.c \
			./app/00t.c

build: 
	${CC} ${SRC_FILES} -o ${TARGET_NAME} \
		-T${SRC_LINKER} \
		-I${INC_DIR} \
		-nostdinc -nostdlib -mcmodel=medany -fno-builtin
	${OBJCOPY} --strip-all ${TARGET_NAME} -O binary ${TARGET_NAME}.bin

dump:
	${OBJDUMP} -h ${TARGET_NAME}



