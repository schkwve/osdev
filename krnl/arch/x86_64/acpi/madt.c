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

#include <arch.h>
#include <debug/log.h>

#include <stdint.h>
#include <string.h>

uint8_t lapic_ids[256] = { 0 };
uint8_t ncores = 0;
uint64_t lapic_ptr = 0;
uint64_t ioapic_ptr = 0;

madt_t *madt;

void acpi_madt_init(void *rsdt_addr)
{
	rsdt_t *rsdt = (rsdt_t *)rsdt_addr;
	int entries = (rsdt->header.len - sizeof(rsdt->header)) / 4;

	for (int i = 0; i < entries; i++) {
		sdt_t *header = (sdt_t *)rsdt->ptr[i];
		if (strncmp(header->sig, "APIC", 4) == 0) {
			madt = (madt_t *)rsdt->ptr[i];
			uint8_t *madt_hdr = (uint8_t *)header;
			lapic_ptr = (uint64_t)((uint32_t)(madt_hdr + 0x24));

			uint8_t *madt_len = madt_hdr + *((uint32_t *)(madt_hdr + 0x04));
			for (madt_hdr += 44; madt_hdr < madt_len; madt_hdr += madt_hdr[1]) {
				switch (madt_hdr[0]) {
				case 0: // LAPIC
					if (madt_hdr[4] & 1) {
						lapic_ids[ncores++] = madt_hdr[3];
					}
					break;
				case 1: // IOAPIC
					ioapic_ptr = (uint64_t) * ((uint32_t *)(madt_hdr + 0x04));
					break;
				case 5: // 64-bit LAPIC
					lapic_ptr = *((uint64_t *)(madt_hdr + 0x04));
				}
			}

			klog(
				"Found %d cores, IOAPIC @ 0x%lx, LAPIC @ 0x%lx, Processor IDs:",
				ncores, ioapic_ptr, lapic_ptr);
			for (int i = 0; i < ncores; i++) {
				klog(" %d", lapic_ids[i]);
			}
			klog("\n");
			return;
		}
	}
}