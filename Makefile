ARCH			= x86_64
CC				= clang
EXEC			= kaos
KVM				= false
LIBS			= /usr/lib
				# Debian [x86_64] : /usr/lib
				# Fedora [x86_64] : /usr/lib64
EFIINC			= /usr/include/efi
EFIINCS			= -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
EFI_CRT_OBJS	= $(LIBS)/crt0-efi-$(ARCH).o
				# Debian : $(LIBS)/...
				# Fedora : $(LIBS)/gnuefi/...
EFI_LDS			= $(LIBS)/elf_$(ARCH)_efi.lds
				# Debian : $(LIBS)/...
				# Fedora : $(LIBS)/gnuefi/...
CFLAGS			= $(EFIINCS) -xc -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Wall -Wno-incompatible-library-redeclaration -O2
ifeq ($(ARCH),x86_64)
	CFLAGS		+= -DHAVE_USE_MS_ABI
endif
LDFLAGS			= -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L $(LIBS) $(EFI_CRT_OBJS)

OVMF			= /usr/share/ovmf/OVMF.fd
				# Debian : /usr/share/ovmf/OVMF.fd
				# Fedora : /usr/share/edk2/ovmf/OVMF_CODE.fd
QEMU			= qemu-system-$(ARCH)
QEMU_OPTS		= -cpu Haswell -smp cores=2,threads=2 -m size=2048 -usb -net none -serial stdio -k fr -name $(EXEC)
ifeq ($(KVM),true)
	QEMU_OPTS	+= -enable-kvm
endif
SHELL			= /bin/bash


all: $(EXEC).efi tools

.PHONY: tools
tools:
	$(MAKE) -C tools/src

run:
	@$(QEMU) -bios $(OVMF) -drive file=dist/$(EXEC).img,if=ide,format=raw $(QEMU_OPTS) #2>/dev/null

build: $(EXEC).img

buildrun: $(EXEC).img run

$(EXEC).img: data.img
	@dd if=/dev/zero of=dist/$@ bs=512 count=93750 status=none
	@/sbin/parted dist/$@ -s -a minimal mklabel gpt
	@/sbin/parted dist/$@ -s -a minimal mkpart EFI FAT16 2048s 93716s
	@/sbin/parted dist/$@ -s -a minimal toggle 1 boot
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

$(EXEC).so: kernel.o boot.o cpu.o memory.o gui.o lib/string.o drivers/io.o drivers/display.o drivers/keyboard.o
	@ld.gold $(LDFLAGS) -o src/$@ src/lib/*.o src/drivers/*.o src/*.o -lefi -lgnuefi

kernel.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/kernel.c

boot.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/boot.c

cpu.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/cpu.c

memory.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/memory.c

gui.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/gui.c

lib/string.o:
	@$(CC) $(CFLAGS) -o src/$@ -c src/lib/string.c

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
	@rm -rf src/lib/*.o
	@rm -rf src/lib/*.so
	@rm -rf src/drivers/*.o
	@rm -rf src/drivers/*.so

clean-tools:
	$(MAKE) clean -C tools/src

mrproper: clean
	@rm -rf dist
	$(MAKE) mrproper -C tools/src
