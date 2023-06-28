/**
 * OSDev
 * Copyright (C) 2023 Jozef Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __VIRT_H_
#define __VIRT_H_

#include <boot/limine.h>

#include <stdint.h>
#include <stdbool.h>

#define VIRT_FLAG_PRESENT (1 << 0)
#define VIRT_FLAG_READWRITE (1 << 1)
#define VIRT_FLAG_USER (1 << 2)
#define VIRT_FLAG_WRITETHROUGH (1 << 3)
#define VIRT_FLAG_CACHE_DISABLE (1 << 4)
#define VIRT_FLAG_WRITECOMBINE (1 << 7)

#define VIRT_FLAGS_DEFAULT (VIRT_FLAG_PRESENT | VIRT_FLAG_READWRITE)
#define VIRT_FLAGS_MMIO (VIRT_FLAGS_DEFAULT | VIRT_FLAG_CACHE_DISABLE)
#define VIRT_FLAGS_USERMODE (VIRT_FLAGS_DEFAULT | VIRT_FLAG_USER)

#define PAGE_TABLE_ENTRIES 512

#define MAKE_TABLE_ENTRY(address, flags) ((address & ~(0xfff)) | flags)

typedef struct {
	uint64_t *pml4;
} addrspace_t;

void virt_init(struct limine_memmap_response *mmap,
			   struct limine_kernel_address_response *kernel);

void virt_map_page(addrspace_t *addrspace, uint64_t vaddr, uint64_t paddr,
				   uint64_t flags);
void virt_unmap_page(addrspace_t *addrspace, uint64_t vaddr);

void virt_map(addrspace_t *addrspace, uint64_t vaddr, uint64_t paddr,
			  uint64_t np, uint64_t flags, bool us);
void virt_unmap(addrspace_t *addrspace, uint64_t vaddr, uint64_t np, bool us);
uint64_t virt_get_paddr(addrspace_t *addrspace, uint64_t vaddr);

#endif // __VIRT_H_