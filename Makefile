#
# OSDev
# Copyright (C) 2023 Jozef Nagy
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#

ARCH := x86_64

AS := nasm
CC := x86_64-elf-gcc
LD := x86_64-elf-ld

ASFLAGS := -felf64

CFLAGS :=	-std=c99 \
			-ffreestanding \
			-fno-stack-protector \
			-fno-stack-check \
			-fno-lto \
			-fno-pie \
			-fno-pic \
			-m64 \
			-march=x86-64 \
			-mabi=sysv \
			-mno-80387 \
			-mno-mmx \
			-mno-sse \
			-mno-sse2 \
			-mno-red-zone \
			-mcmodel=kernel \
			-MMD \
			-D$(ARCH) \
			-Ikrnl/ \
			-Ikrnl/arch \
			-Icorelib

LDFLAGS :=	-nostdlib \
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

C_FILES=$(shell find krnl corelib -name "*.c" -type f)
AS_FILES=$(shell find krnl corelib -name "*.asm" -type f)

INCFILES=$(C_FILES:.c=.d)
OBJ=$(C_FILES:.c=.o) $(AS_FILES:.asm=.o)


.PHONY: all
all: clean $(KERNEL) $(ISO) # Build everything

.PHONY: iso
iso: $(ISO) # Build a bootable ISO

.PHONY: run
run: $(ISO) # Run OSDev
	@qemu-system-x86_64 -serial stdio -smp 4 -no-reboot -M smm=off -m 512M -cdrom $(ISO) -display none

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
	@rm -rf $(INCFILES) $(OBJ) $(KERNEL) $(ISO)
