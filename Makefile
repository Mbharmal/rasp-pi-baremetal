# Change the path of the toolchain according to the install path
#GCCPATH = ../../gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin
GCCPATH =

LINKER_SCRIPT = linker_script.ld

C_FLAGS = -Wall -O2 -nostartfiles -nostdlib -nostdinc -ffreestanding
LINKER_FLAGS = -nostdlib -T $(LINKER_SCRIPT)

all: clean kernel8.img

start.o: start.S
	aarch64-linux-gnu-gcc $(C_FLAGS) -c start.S -o start.o

basic_io.o: basic_io.c
	aarch64-linux-gnu-gcc $(GCCFC_FLAGSLAGS) -c basic_io.c -o basic_io.o

main.o: main.c
	aarch64-linux-gnu-gcc $(C_FLAGS) -c main.c -o main.o

kernel8.img: start.o basic_io.o main.o
	aarch64-linux-gnu-ld $(LINKER_FLAGS) start.o main.o basic_io.o -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm -rf kernel8.elf *.o *.img
