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

#ifndef __PHYS_H_
#define __PHYS_H_

#include <stdint.h>

#define PAGE_SIZE 0x1000

#define LVL5_PAGING_HIGHER_HALF 0xFF00000000000000UL
#define LVL4_PAGING_HIGHER_HALF 0xFFFF800000000000UL

#define ALIGN_DOWN(addr, align) ((addr) & ~((align)-1))
#define ALIGN_UP(addr, align) (((addr) + (align)-1) & ~((align)-1))

typedef struct {
	uint64_t total_mem;
	uint64_t used_mem;
	uint64_t free_mem;
} kmem_info_t;

typedef struct {
	uint64_t *bitmap;
	size_t size;
} bitmap_t;

static inline uintptr_t phys_to_higher_half(uintptr_t addr);

void bitmap_set(uint64_t bit);

void pmm_init(struct limine_memmap_response *mmap);

#endif // __PHYS_H_