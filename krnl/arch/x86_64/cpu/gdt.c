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

// This code is based off of https://github.com/NSG650/Polaris

#include <arch.h>

#include <debug/log.h>

gdtr_t gdt;
gdtptr_t gdtptr;

void gdt_init()
{
	gdt_set_entry(0, 0, 0, 0, 0);
	gdt_set_entry(1, 0, 0, 0x9A, 0x20);
	gdt_set_entry(2, 0, 0, 0x92, 0);
	gdt_set_entry(3, 0, 0, 0xF2, 0);
	gdt_set_entry(4, 0, 0, 0xF2, 0x20);

	gdt.tss.size = sizeof(tss_t);
	gdt.tss.flags1 = 0x89;

	gdtptr.size = sizeof(gdt) - 1;
	gdtptr.offset = (uint64_t)&gdt;

	klog("Loading GDT...");

	gdt_load();
	tss_load();
}

void gdt_set_entry(int entry, uint16_t limit, uint32_t base, uint8_t access,
				   uint8_t flags)
{
	gdt.entry[entry].limit = limit;

	gdt.entry[entry].base_lo = (base >> 8) & 0xFF;
	gdt.entry[entry].base_mid = (base >> 16) & 0xFF;
	gdt.entry[entry].base_hi = (base >> 24) & 0xFF;

	gdt.entry[entry].access = access;
	gdt.entry[entry].flags = flags;
}