ARCH			= x86_64
CC				= clang
EXEC			= kaos
KVM				= false

EFIINC			= /usr/include/efi
EFIINCS			= -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
EFI_CRT_OBJS	= /usr/lib/crt0-efi-$(ARCH).o
EFI_LDS			= /usr/lib/elf_$(ARCH)_efi.lds
CFLAGS			= $(EFIINCS) -xc -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Wall -Wno-incompatible-library-redeclaration -O2
ifeq ($(ARCH),x86_64)
	CFLAGS		+= -DHAVE_USE_MS_ABI
endif
LDFLAGS			= -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L /usr/lib $(EFI_CRT_OBJS)

OVMF			= /usr/share/ovmf/OVMF.fd
QEMU			= qemu-system-$(ARCH)
QEMU_OPTS		= -cpu qemu64 -m 128
ifeq ($(KVM),true)
	QEMU_OPTS	+= -enable-kvm
endif
SHELL			= /bin/bash


all: $(EXEC).efi tools

.PHONY: tools
tools:
	$(MAKE) -C src/tools

run: $(EXEC)-qemu.img
	@$(QEMU) -bios $(OVMF) -drive file=dist/$(EXEC)-qemu.img,if=ide,format=raw $(QEMU_OPTS) 

$(EXEC)-qemu.img: data.img
	@dd if=/dev/zero of=dist/$@ bs=512 count=93750 status=none
	@sudo parted dist/$@ -s -a minimal mklabel gpt
	@sudo parted dist/$@ -s -a minimal mkpart EFI FAT16 2048s 93716s
	@sudo parted dist/$@ -s -a minimal toggle 1 boot
	@dd if=/tmp/$< of=dist/$@ bs=512 count=91669 seek=2048 conv=notrunc status=none

data.img: $(EXEC).efi
	@dd if=/dev/zero of=/tmp/$@ bs=512 count=91669 status=none
	@mformat -i /tmp/$@ -h 32 -t 32 -n 64 -c 1
	@mmd -i /tmp/data.img ::/EFI
	@mmd -i /tmp/data.img ::/EFI/BOOT
	@mcopy -i /tmp/$@ dist/$< ::/EFI/BOOT/BOOTX64.efi
	
$(EXEC).efi: $(EXEC).so
	@mkdir -p dist
	@objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .reloc --target=efi-app-$(ARCH) src/$< dist/$@

$(EXEC).so: main.o memory.o drivers/io.o drivers/display.o drivers/keyboard.o
	@ld.gold $(LDFLAGS) src/$< src/drivers/*.o -o src/$@ -lefi -lgnuefi

main.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/main.c
	
memory.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/memory.c

drivers/io.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/drivers/io.c

drivers/display.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/drivers/display.c

drivers/keyboard.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/drivers/keyboard.c

.PHONY: clean
clean:
	@rm -rf src/*.o
	@rm -rf src/*.so
	@rm -rf src/drivers/*.o
	@rm -rf src/drivers/*.so
	
clean-tools:
	$(MAKE) clean -C src/tools

mrproper: clean
	@rm -rf dist
	$(MAKE) mrproper -C src/tools

