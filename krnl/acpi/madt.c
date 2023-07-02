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
#include <acpi/madt.h>
#include <acpi/rsdt.h>
#include <acpi/sdt.h>

#include <dd/apic/apic.h>
#include <dd/apic/lapic.h>

#include <debug/log.h>

#include <stdint.h>
#include <string.h>

uint32_t g_cpu_cnt;
uint8_t g_cpu_ids[MAX_CPU_COUNT];

apic_iso_t *g_apic_iso[16];
uint8_t g_apic_isos = 0;

madt_t *g_madt;

void acpi_madt_init(madt_t *_madt)
{
	g_madt = _madt;

	g_lapic_addr = (uint8_t *)(uintptr_t)g_madt->lapic_addr;
	klog("found LAPIC @ 0x%08x\n", g_madt->lapic_addr);

	uint8_t *ptr = (uint8_t *)(g_madt+1);
	uint8_t *end = (uint8_t *)(g_madt) + g_madt->hdr.len;


	while (ptr < end) {
		apic_hdr_t *hdr = (apic_hdr_t *)ptr;
		
		switch (hdr->type) {
			case ACPI_MADT_LAPIC:
				apic_lapic_t *lapic = (apic_lapic_t *)ptr;
				if (g_cpu_cnt < MAX_CPU_COUNT) {
					g_cpu_ids[g_cpu_cnt] = lapic->lapic_id;
					++g_cpu_cnt;
				}
				klog("found CPU id = %d %d %x\n", lapic->proc_id, lapic->lapic_id, lapic->flags);
				break;
			case ACPI_MADT_IOAPIC:
				apic_ioapic_t *ioapic = (apic_ioapic_t *)ptr;
				g_ioapic_addr = (uint8_t *)(uintptr_t)ioapic->ioapic_addr;
				klog("found IOAPIC id = %d, address = 0x%08x, gsi = %d\n", ioapic->ioapic_id, ioapic->ioapic_addr, ioapic->gsi_base);
				break;
			case ACPI_MADT_ISO:
				apic_iso_t *iso = (apic_iso_t *)ptr;
				g_apic_iso[g_apic_isos++] = (apic_iso_t *)ptr;
				klog("found interrupt override bus_src = %d, gsi = %d, interrupt = %d, flags = 0x%04x\n", g_apic_iso[g_apic_isos-1]->bus_src, g_apic_iso[g_apic_isos-1]->gsi, g_apic_iso[g_apic_isos-1]->irq_src, g_apic_iso[g_apic_isos-1]->flags);
				break;
			default:
				klog("unknown APIC structure type = %d\n", hdr->type);
				break;
		}

		ptr += hdr->len;
	}
}