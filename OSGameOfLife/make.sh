#!/bin/bash
rm *.iso *.bin
SOURCES=`find -name "*.c"`
i686-elf-as boot.s -o boot.o
for sourcefile in $SOURCES
do
i686-elf-gcc -c $sourcefile -o $sourcefile.o -std=gnu99 -ffreestanding -Og -Wall -Wextra -w
done
OBJECTS=`find -name "*.o"`
i686-elf-gcc -T linker.ld -o os.bin -ffreestanding -Og -nostdlib $OBJECTS -lgcc
rm *.o
qemu-system-i386 -kernel os.bin
