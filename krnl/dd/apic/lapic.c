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

#include <dd/apic/pic.h>
#include <dd/apic/lapic.h>
#include <dd/apic/ioapic.h>

#include <mm/phys.h>
#include <mm/virt.h>

#include <debug/log.h>
#include <utils/utils.h>

#include <stdint.h>
#include <stdbool.h>

uint8_t *g_lapic_addr;

void lapic_init()
{
	//g_lapic_addr = PHYS_TO_VIRT(g_lapic_addr);
	//virt_map(NULL, (uint64_t)g_lapic_addr, VIRT_TO_PHYS(g_lapic_addr), 1, VIRT_FLAGS_MMIO, true);

	lapic_out(LAPIC_TPR, 0);
	lapic_out(LAPIC_DFR, 0xFFFFFFFF);
	lapic_out(LAPIC_LDR, 0x01000000);
	lapic_out(LAPIC_SVR, 0x100 | LAPIC_ENABLE);
	klog("done\n");
}

uint32_t lapic_get_id()
{
	lapic_in(LAPIC_ID) >> 24;
}

uint32_t lapic_in(uint32_t reg)
{
	return mmio_inw(g_lapic_addr + reg);
}

void lapic_out(uint32_t reg, uint32_t data)
{
	mmio_outw(g_lapic_addr + reg, data);
}