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

#include <stdint.h>

void pmm_init(struct limine_memmap_response *mmap)
{
	// fill kmem_info
	kmem_info_t mem_info;
	mem_info.total_mem = 0;

	for (uint64_t i = 0; i < mmap->entry_count; i++) {
		struct limine_memmap_entry *entry = mmap->entries[i];
		klog("Entry %i  | Base: 0x%.16llx | Length: %llu (0x%llx) | Type: %d\n", i, entry->base, entry->length, entry->length, entry->type);
		if (entry->type != LIMINE_MEMMAP_USABLE) continue;
		mem_info.total_mem += entry->length;
	}

	klog("Total Memory: %llu MB\n", (mem_info.total_mem / 1024 / 1024));

	// Map all memory as used except the usable memory
	for (uint64_t i = 0; i < mmap->entry_count; i++) {
		struct limine_memmap_entry *entry = mmap->entries[i];
		if (entry == LIMINE_MEMMAP_USABLE) continue;

		//bitmap_set(entry);
	}
}