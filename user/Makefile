
CROSS_PREFIX = riscv64-unknown-elf-
CC = ${CROSS_PREFIX}gcc

OBJCOPY = rust-objcopy
OBJDUMP = rust-objdump

INC_DIR = ./include
APP_NAME = 00t
SRC_LINKER = ./linker/${APP_NAME}_linker.ld
TARGET_NAME = ./target/${APP_NAME}

SRC_FILES = ./start.s \
			./main.c \
			./syscall.c \
			./lib/printf.c \
			./lib/string.c \
			./app/${APP_NAME}.c

build: 
	${CC} ${SRC_FILES} -o ${TARGET_NAME} \
		-T${SRC_LINKER} \
		-I${INC_DIR} \
		-nostdinc -nostdlib -mcmodel=medany -fno-builtin
	${OBJCOPY} --strip-all ${TARGET_NAME} -O binary ${TARGET_NAME}.bin

dump:
	${OBJDUMP} -h ${TARGET_NAME}


clean:
	rm ./target/*



