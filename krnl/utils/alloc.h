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

#ifndef __ALLOC_H_
#define __ALLOC_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t numpages;
	size_t size;
} memory_metadata_t;

void *kalloc(uint64_t size);
void kfree(void *addr);
void *krealloc(void *addr, size_t newsize);

#endif // __ALLOC_H_