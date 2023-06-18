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

extern madt_t *madt;

void acpi_init()
{
	void *rsdp = acpi_rsdp_init();
	acpi_madt_init(rsdp);
}

uint32_t acpi_remap_irq(uint32_t irq)
{
	uint16_t *ptr = (uint16_t *)(madt + 1);
	uint8_t *end = (uint8_t *)madt + madt->hdr.len;

	while (ptr < end) {
		apic_hdr_t *hdr = (apic_hdr_t *)ptr;
		uint8_t type = hdr->type;
		uint8_t len = hdr->len;

		// Interrupt Override
		if (type == 2) {
			apic_iso_t *iso = (apic_iso_t *)ptr;

			if (iso->src == irq) {
				return iso->interrupt;
			}
		}
		ptr += len;
	}

	return irq;
}