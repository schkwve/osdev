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

#include <mm/bitmap.h>
#include <mm/phys.h>

#include <stdint.h>
#include <stdbool.h>

extern mem_info_t mem_info;

void bitmap_set(uint64_t *addr, int bit)
{
	addr[bit/8] |= (1 << (bit % 8));
}

bool bitmap_get(uint64_t *addr, int bit)
{
	return addr[bit/8] & (1 << (bit % 8));
}

void bitmap_set_page(uint64_t addr, uint64_t numpages)
{
	for (uint64_t i = addr; i < addr + (numpages * PAGE_SIZE); i += PAGE_SIZE) {
		mem_info.bitmap[i / (PAGE_SIZE * BMP_PAGES_PER_BYTE)] &=
			~((1 << ((i / PAGE_SIZE) % BMP_PAGES_PER_BYTE)));
	}
}

bool bitmap_get_page(uint64_t addr, uint64_t numpages)
{
	bool free = true;
	for (uint64_t i = addr; i < addr + (numpages * PAGE_SIZE); i += PAGE_SIZE) {
		free = mem_info.bitmap[i / (PAGE_SIZE * BMP_PAGES_PER_BYTE)] &
			   (1 << ((i / PAGE_SIZE) % BMP_PAGES_PER_BYTE));
		if (!free)
			break;
	}
	return free;
}