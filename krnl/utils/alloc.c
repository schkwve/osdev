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

#include <utils/alloc.h>

void *kmalloc(size_t size)
{
	void *ptr = NULL;

	size_t new_size = next_pow_two(size);

	ptr = phys_mm_alloc((new_size / PAGE_SIZE) + 1);
	ptr += PAGE_SIZE;

	return ptr;
}

void kfree(void *ptr)
{
	if (!ptr) {
		return;
	}

	phys_mm_free(ptr, (sizeof(ptr) / PAGE_SIZE) + 1);
}

size_t next_pow_two(size_t num)
{
	size_t result = 2;

	while (result < num) {
		result <<= 1;
	}

	return result;
}