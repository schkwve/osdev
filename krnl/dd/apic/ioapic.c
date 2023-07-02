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

#include <acpi/madt.h>

#include <dd/apic/lapic.h>
#include <dd/apic/ioapic.h>

#include <debug/log.h>

#include <mm/phys.h>
#include <mm/virt.h>

#include <stdint.h>
#include <stdbool.h>

uint8_t *g_ioapic_addr;

void ioapic_init()
{
	//g_ioapic_addr = PHYS_TO_VIRT(g_ioapic_addr);
	//virt_map(NULL, (uint64_t)g_ioapic_addr, VIRT_TO_PHYS(g_ioapic_addr), 1, VIRT_FLAGS_MMIO, true);

	uint32_t max_entries = ioapic_in(g_ioapic_addr, IOAPIC_VER);
	uint32_t count = ((max_entries >> 16) & 0xFF) + 1; // max redirection entries

	klog("IOAPIC pins = %d\n", count);
	for (uint32_t i = 0; i < count; ++i) {
		ioapic_set_entry(g_ioapic_addr, i, 1 << 16);
	}

	klog("done\n");
}

void ioapic_set_entry(uint8_t *base, uint8_t index, uint64_t data)
{
	ioapic_out(base, IOREDTBL + index * 2, (uint32_t)data);
	ioapic_out(base, IOREDTBL + index * 2 + 1, (uint32_t)(data >> 32));
}

uint32_t ioapic_in(uint8_t *base, uint8_t reg)
{
	mmio_outw(base + IOREGSEL, reg);
	return mmio_inw(base + IOWIN);
}

void ioapic_out(uint8_t *base, uint8_t reg, uint64_t data)
{
	mmio_outw(base + IOREGSEL, reg);
	mmio_outw(base + IOWIN, data);
}