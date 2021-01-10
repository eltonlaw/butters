NAME=butters
TARGET=i686-elf
CXX=i686-elf-g++
AS=i686-elf-as
CXXFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# quit by: Ctrl+Alt+g > clicking shell > ctrl+c
qemu: $(NAME).iso
	qemu-system-i386 -cdrom $(NAME).iso

$(NAME).iso: $(NAME).bin
	mkdir -p isodir/boot/grub
	cp $(NAME).bin isodir/boot/$(NAME).bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(NAME).iso isodir

$(NAME).bin: boot.o kernel.o
	i686-elf-g++ -T linker.ld -o $(NAME).bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc	

check:
	grub-file --is-x86-multiboot $(NAME).bin

boot.o:
	$(AS) boot.s -o $@

kernel.o:
	$(CXX) -c kernel.cc -o $@ $(CXXFLAGS)

clean:
	rm -f boot.o kernel.o $(NAME).bin $(NAME).iso
