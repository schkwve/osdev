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
#include <mm/bitmap.h>

#include <debug/log.h>

#include <stdint.h>
#include <string.h>

mem_info_t mem_info = { 0 };

void phys_free(uint64_t addr, uint64_t numpages)
{
	for (uint64_t i = addr; i < addr + (numpages * PAGE_SIZE); i += PAGE_SIZE) {
		if (!bitmap_get(i, 1))
			mem_info.free_mem += PAGE_SIZE;

		mem_info.bitmap[i / (PAGE_SIZE * BMP_PAGES_PER_BYTE)] |=
			1 << ((i / PAGE_SIZE) % BMP_PAGES_PER_BYTE);
	}
}

bool phys_alloc(uint64_t addr, uint64_t numpages)
{
	if (!bitmap_get(addr, numpages))
		return false;

	bitmap_set(addr, numpages);
	mem_info.free_mem -= numpages * PAGE_SIZE;
	return true;
}

uint64_t phys_get(uint64_t numpages, uint64_t baseaddr)
{
	for (uint64_t i = baseaddr; i < mem_info.phys_limit; i += PAGE_SIZE) {
		if (phys_alloc(i, numpages))
			return i;
	}

	//panic("Out of Physical Memory");
	klog("Out of physical memory\n");
	for (;;) {
		__asm__ volatile("cli;hlt");
	}
	return 0;
}

void phys_init(struct limine_memmap_response *map)
{
	mem_info.phys_limit = 0;
	mem_info.total_mem = 0;
	mem_info.free_mem = 0;

	for (size_t i = 0; i < map->entry_count; i++) {
		struct limine_memmap_entry *entry = map->entries[i];

		if (entry->type == LIMINE_MEMMAP_RESERVED)
			continue;

		if (entry->type == LIMINE_MEMMAP_USABLE ||
			entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE ||
			entry->type == LIMINE_MEMMAP_ACPI_RECLAIMABLE ||
			entry->type == LIMINE_MEMMAP_KERNEL_AND_MODULES) {
			mem_info.total_mem += entry->length;
		}

		uint64_t new_limit = entry->base + entry->length;

		if (new_limit > mem_info.phys_limit) {
			mem_info.phys_limit = new_limit;
			klog("entry base 0x%x, length %d, type %d\n", entry->base,
				 entry->length, entry->type);
		}
	}

	uint64_t bitmap_size =
		mem_info.phys_limit / (PAGE_SIZE * BMP_PAGES_PER_BYTE);
	bool bitmap_place_found = false;
	for (size_t i = 0; i < map->entry_count; i++) {
		struct limine_memmap_entry *entry = map->entries[i];

		if (entry->base + entry->length <= 0x100000)
			continue;

		if (entry->length >= bitmap_size &&
			entry->type == LIMINE_MEMMAP_USABLE) {
			if (!bitmap_place_found)
				mem_info.bitmap = (uint8_t *)PHYS_TO_VIRT(entry->base);
			bitmap_place_found = true;
		}
	}

	memset(mem_info.bitmap, 0, bitmap_size);
	klog("memory bitmap address: 0x%x\n", mem_info.bitmap);

	// populate the bitmap
	for (size_t i = 0; i < map->entry_count; i++) {
		struct limine_memmap_entry *entry = map->entries[i];

		if (entry->base + entry->length <= 0x100000)
			continue;

		if (entry->type == LIMINE_MEMMAP_USABLE)
			phys_free(entry->base, NUM_PAGES(entry->length));
	}

	// mark the bitmap as used
	phys_alloc(VIRT_TO_PHYS(mem_info.bitmap), NUM_PAGES(bitmap_size));

	klog("memory total: %d, phys limit: %d (0x%x), free: %d, used: %d\n",
		 mem_info.total_mem, mem_info.phys_limit, mem_info.phys_limit,
		 mem_info.free_mem, mem_info.total_mem - mem_info.free_mem);
	klog("done\n");
}

uint64_t phys_get_total_mem(void)
{
	return mem_info.total_mem / (1024 * 1024);
}

uint64_t phys_get_free_mem(void)
{
	return mem_info.free_mem / (1024 * 1024);
}

uint64_t phys_get_used_mem(void)
{
	return (mem_info.total_mem - mem_info.free_mem) / (1024 * 1024);
}