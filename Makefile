ARCH := x86_64

AS := nasm
CC := x86_64-elf-gcc
LD := x86_64-elf-ld

ASFLAGS := -felf64

CFLAGS := -std=c99             \
			-ffreestanding       \
			-fno-stack-protector \
			-fno-stack-check     \
			-fno-lto             \
			-fno-pie             \
			-fno-pic             \
			-m64                 \
			-march=x86-64        \
			-mabi=sysv           \
			-mno-80387           \
			-mno-mmx             \
			-mno-sse             \
			-mno-sse2            \
			-mno-red-zone        \
			-mcmodel=kernel      \
			-MMD                 \

LDFLAGS := -nostdlib \
			-static \
			-no-pie \
			-melf_x86_64 \
			-zmax-page-size=0x1000 \
			-Tkrnl/arch/$(ARCH)/linker.ld

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG -g3
endif

KERNEL=krnl.sys
ISO=osdev.iso

C_FILES=$(shell find krnl -name "*.c" -type f)
AS_FILES=$(shell find krnl -name "*.asm" -type f)
OBJ=$(C_FILES:.c=.o) $(AS_FILES:.asm=.o)

.PHONY: all
all: clean $(KERNEL) $(ISO) # Build everything

.PHONY: iso
iso: $(ISO) # Build a bootable ISO

.PHONY: run
run: $(ISO) # Run OSDev
	@qemu-system-x86_64 -m 512M -cdrom $(ISO)

.PHONY: format
format: # Format the code as described in .clang-format
	find krnl -type f -name "*.c" -o -name "*.h" | xargs clang-format -i

.PHONY: help
help: # Print help
	@grep '^[^.#]\+:\s\+.*#' Makefile | \
		gsed "s/\(.\+\):\s*\(.*\) #\s*\(.*\)/`printf "\033[93m"`\1`printf "\033[0m"`	\3 [\2]/" | \
		expand -t20

%.o: %.c
	@printf " CC  $^\n"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	@printf " AS  $^\n"
	@$(AS) $(ASFLAGS) $< -o $@

$(KERNEL): $(OBJ)
	@printf " LD  $@\n"
	@$(LD) $(OBJ) $(LDFLAGS) -o $@

$(ISO): $(KERNEL)
	@printf " ISO $@\n"
	@mkdir -p iso_tmp/EFI/BOOT
	@cp $(KERNEL) limine.cfg ex/limine/limine.sys ex/limine/limine-cd.bin ex/limine/limine-cd-efi.bin iso_tmp/
	@xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-cd-efi.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_tmp -o $(ISO)
	@ex/limine/limine-deploy $(ISO)
	@rm -rf iso_tmp/

.PHONY: clean
clean: # Clean generated files
	@rm -rf $(OBJ) $(KERNEL) $(ISO)
