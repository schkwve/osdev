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

#include <boot/limine.h>

#include <stdint.h>
#include <stdbool.h>

#define PAGE_SIZE 4096
#define BMP_PAGES_PER_BYTE 8

#define MEM_VIRT_OFFSET 0xffff800000000000

#define VIRT_TO_PHYS(a) (((uint64_t)(a)) - MEM_VIRT_OFFSET)
#define PHYS_TO_VIRT(a) (((uint64_t)(a)) + MEM_VIRT_OFFSET)

#define KERNEL_CODE_OFFSET 0xffffffff80200000

#define NUM_PAGES(num) (((num) + PAGE_SIZE - 1) / PAGE_SIZE)
#define PAGE_ALIGN_UP(num) (NUM_PAGES(num) * PAGE_SIZE)

#define MAX_MEM_TOTAL_SIZE (128 * GB)
#define MAX_MEM_BMP_SIZE (NUM_PAGES(MAX_MEM_TOTAL_SIZE) / BMP_PAGES_PER_BYTE)

typedef struct {
	uint64_t phys_limit;
	uint64_t total_mem;
	uint64_t free_mem;

	uint8_t *bitmap;
} mem_info_t;

typedef struct {
	uint64_t vaddr;
	uint64_t paddr;
	uint64_t flags;
	uint64_t np;
} mem_map_t;

void phys_init(struct limine_memmap_response *map);

uint64_t phys_get(uint64_t numpages, uint64_t baseaddr);

bool phys_alloc(uint64_t addr, uint64_t numpages);
void phys_free(uint64_t addr, uint64_t numpages);

uint64_t phys_get_total_mem(void);
uint64_t phys_get_free_mem(void);
uint64_t phys_get_used_mem(void);

#endif // __PHYS_H_