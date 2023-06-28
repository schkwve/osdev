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

#include <cpu/gdt.h>
#include <debug/log.h>

#include <stdint.h>

static tss_t tss;
static gdt_desc_t gdt[10];
static gdtr_t gdtr;

void gdt_set_entry(int entry, uint16_t limit, uint32_t base, uint8_t type,
				   uint8_t flags)
{
	gdt[entry].limit = limit;

	gdt[entry].base_lo = (base >> 8) & 0xFF;
	gdt[entry].base_mid = (base >> 16) & 0xFF;
	gdt[entry].base_hi = (base >> 24) & 0xFF;

	gdt[entry].type = type;
	gdt[entry].flags = flags;
}

void gdt_init(void)
{
	gdt_set_entry(0, 0, 0, 0, 0);
	gdt_set_entry(1, 0, 0, 0x9A, 0xA0);
	gdt_set_entry(2, 0, 0, 0x92, 0xA0);
	gdt_set_entry(3, 0, 0, 0, 0);
	gdt_set_entry(4, 0, 0, 0x92, 0xA0);
	gdt_set_entry(5, 0, 0, 0x9A, 0xA0);
	gdt_set_entry(6, 0, 0, 0x92, 0xA0);
	gdt_set_entry(7, 0, 0, 0x9A, 0xA0);
	gdt_set_entry(8, 0, 0, 0x89, 0xA0);
	gdt_set_entry(9, 0, 0, 0, 0);

	for (uint64_t i = 0; i < sizeof(tss); i++)
		((uint8_t *)(void *)&tss)[i] = 0;

	uint64_t tss_base = ((uint64_t)&tss);

	gdt[8].base_lo = tss_base & 0xFFFF;
	gdt[8].base_mid = (tss_base >> 16) & 0xFF;
	gdt[8].base_hi = (tss_base >> 24) & 0xFF;
	gdt[8].limit = sizeof(tss);
	gdt[9].limit = (tss_base >> 32) & 0xFFFF;
	gdt[9].base_lo = (tss_base >> 48) & 0xFFFF;

	gdtr.size = sizeof(gdt) - 1;
	gdtr.offset = (uint64_t)&gdt;

	gdt_load(&gdtr);

	klog("done\n");
}