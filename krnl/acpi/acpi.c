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

#include <acpi/acpi.h>
#include <acpi/rsdt.h>
#include <acpi/madt.h>

#include <dd/apic/apic.h>

#include <debug/log.h>

void acpi_init()
{
	acpi_rsdp_init();

	klog("done\n");
}

uint32_t acpi_remap_irq(uint32_t irq)
{
	for (int i = 0; i < g_apic_isos; i++) {
		if (g_apic_iso[i]->irq_src == irq) {
			return g_apic_iso[i]->gsi;
		}
	}

	return irq;
}