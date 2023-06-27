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

#include <boot/limine.h>
#include <mm/phys.h>

#include <debug/log.h>
#include <utils/utils.h>

#include <stdint.h>
#include <string.h>

kmem_info_t mem_info;
bitmap_t bitmap;

uintptr_t phys_to_higher_half(uintptr_t addr)
{
	if ((read_cr4() >> 12) & 1) {
		return LVL5_PAGING_HIGHER_HALF + addr;
	}

	return LVL4_PAGING_HIGHER_HALF + addr;
}

uintptr_t higher_half_to_phys(uintptr_t addr)
{
	if ((read_cr4() >> 12) & 1) {
		return LVL5_PAGING_HIGHER_HALF - addr;
	}

	return LVL4_PAGING_HIGHER_HALF - addr;
}

void bitmap_set(int bit)
{
	bitmap.bitmap[bit / 8] |= (1 << (bit % 8));
}

void bitmap_clear(int bit)
{
	bitmap.bitmap[bit / 8] &= ~(1 << (bit % 8));
}

uint8_t bitmap_get(int bit)
{
	return bitmap.bitmap[bit / 8] & (1 << (bit % 8));
}

void phys_mm_init(struct limine_memmap_response *mmap)
{
	// fill kmem_info
	mem_info.total_mem = 0;
	mem_info.used_mem = 0;

	size_t highest_page = 0;

	for (uint64_t i = 0; i < mmap->entry_count; i++) {
		struct limine_memmap_entry *entry = mmap->entries[i];

		klog("Entry %i | Base: 0x%.12llx | Length: %llu (0x%llx) | Type: %d\n",
			 i, entry->base, entry->length, entry->length, entry->type);

		size_t top = entry->base + entry->length;
		if (top > highest_page)
			highest_page = top;

		if (entry->type != LIMINE_MEMMAP_USABLE)
			continue;

		mem_info.total_mem += entry->length;
	}

	klog("Total Memory: %llu MB\n", (mem_info.total_mem / 1024 / 1024));

	bitmap.size = ALIGN_UP(ALIGN_DOWN(highest_page, PAGE_SIZE) / PAGE_SIZE / 8,
						   PAGE_SIZE);

	// Look for big enough memory to put the bitmap in
	for (uint64_t i = 0; i < mmap->entry_count; i++) {
		struct limine_memmap_entry *entry = mmap->entries[i];

		if (entry->type != LIMINE_MEMMAP_USABLE)
			continue;

		if (entry->length > bitmap.size) {
			bitmap.bitmap = (uint64_t *)(phys_to_higher_half(entry->base));
			entry->base += bitmap.size;
			entry->length -= bitmap.size;
			break;
		}
	}

	// Map all memory as used except the usable memory
	mem_info.used_mem = mem_info.total_mem;
	memset((void *)bitmap.bitmap, 0xFF, bitmap.size);

	for (uint64_t i = 0; i < mmap->entry_count; i++) {
		struct limine_memmap_entry *entry = mmap->entries[i];
		if (entry->type == LIMINE_MEMMAP_USABLE) {
			phys_mm_free((void *)entry->base, entry->length / PAGE_SIZE);
		}
	}

	bitmap_set(0);

	klog("PMM Init\n");
}

void *phys_mm_alloc(size_t pages)
{
	if (pages == 0)
		return NULL;

	for (size_t i = 0; i < pages; i++) {
		for (size_t j = 0; j < (pages / PAGE_SIZE); i++) {
			if (!bitmap_get(i)) {
				return (void *)(pages * PAGE_SIZE);
			}
		}
	}

	return NULL;
}

void phys_mm_free(void *ptr, size_t pages)
{
	uint64_t index = higher_half_to_phys((uint64_t)ptr / PAGE_SIZE);
	for (size_t i = 0; i < pages; i++) {
		bitmap_clear(index + i);
	}

	mem_info.used_mem -= pages;
}