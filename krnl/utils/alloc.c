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

#include <mm/phys.h>
#include <mm/virt.h>

#include <utils/alloc.h>

#include <stdint.h>
#include <string.h>

void *kalloc(uint64_t size)
{
	memory_metadata_t *alloc =
		(memory_metadata_t *)PHYS_TO_VIRT(phys_get(NUM_PAGES(size) + 1, 0x0));

	alloc->numpages = NUM_PAGES(size);
	alloc->size = size;

	return ((uint8_t *)alloc) + PAGE_SIZE;
}

void kfree(void *addr)
{
	memory_metadata_t *d = (memory_metadata_t *)((uint8_t *)addr - PAGE_SIZE);

	phys_free(VIRT_TO_PHYS(d), d->numpages + 1);
}

void *krealloc(void *addr, size_t newsize)
{
	if (!addr)
		return kalloc(newsize);

	memory_metadata_t *d = (memory_metadata_t *)((uint8_t *)addr - PAGE_SIZE);

	if (NUM_PAGES(d->size) == NUM_PAGES(newsize)) {
		d->size = newsize;
		d->numpages = NUM_PAGES(newsize);
		return addr;
	}

	void *new = kalloc(newsize);
	if (d->size > newsize)
		memcpy(new, addr, newsize);
	else
		memcpy(new, addr, d->size);

	kfree(addr);
	return new;
}