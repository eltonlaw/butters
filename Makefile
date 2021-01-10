NAME=butters
TARGET=i686-elf
CXX=i686-elf-g++
AS=i686-elf-as
CXXFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

CPPFILES := $(shell find ./ -type f -name '*.cpp')
OBJ    := $(CPPFILES:.cpp=.o)

iso: $(NAME).iso
bin: $(NAME).bin

# quit by: Ctrl+Alt+g > clicking shell > ctrl+c
qemu: $(NAME).iso
	qemu-system-i386 -cdrom $(NAME).iso

$(NAME).iso: $(NAME).bin
	mkdir -p isodir/boot/grub
	cp $(NAME).bin isodir/boot/$(NAME).bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(NAME).iso isodir

$(NAME).bin: $(OBJ) boot.o
	i686-elf-g++ -T  linker.ld -o $(NAME).bin $^ -ffreestanding -O2 -nostdlib -lgcc

check:
	grub-file --is-x86-multiboot $(NAME).bin

boot.o: boot.s
	$(AS) boot.s -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f boot.o kernel.o $(NAME).bin $(NAME).iso
