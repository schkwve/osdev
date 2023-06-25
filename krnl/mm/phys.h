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

typedef struct {
	uint64_t total_mem;
	uint64_t used_mem;
	uint64_t free_mem;
} kmem_info_t;

void pmm_init(struct limine_memmap_response *mmap);

#endif // __PHYS_H_