NAME=butters
TARGET=i686-elf
CXX=i686-elf-g++
AS=i686-elf-as
CXXFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

$(NAME).bin: boot.o kernel.o
	i686-elf-g++ -T linker.ld -o $(NAME).bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc	

check:
	grub-file --is-x86-multiboot $(NAME).bin

boot.o:
	$(AS) boot.s -o $@

kernel.o:
	$(CXX) -c kernel.cc -o $@ $(CXXFLAGS)

clean:
	rm -f boot.o kernel.o $(NAME).bin
