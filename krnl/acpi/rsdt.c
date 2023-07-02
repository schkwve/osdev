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
#include <acpi/rsdt.h>
#include <acpi/madt.h>

#include <debug/log.h>

#include <stdint.h>
#include <string.h>

void acpi_rsdp_init(struct limine_rsdp_response *response)
{
	uint64_t *rsdp_addr = response->address;

	// Validate RSDP
	uint8_t checksum = 0;

	for (uint8_t i = 0; i < 20; i++) {
		checksum += ((uint8_t *)rsdp_addr)[i];
	}

	if ((checksum & 0xFF) != 0) {
		klog("invalid RSDP Checksum!\n");
		//panic("Invalid RSDP Checksum!\n");
		for (;;) {
			__asm__ volatile("cli;hlt");
		}
	}

	rsdp_t *rsdp = (rsdp_t *)rsdp_addr;
	klog("OEM: %s\n", rsdp->oem_id);
	if (rsdp->rev >= 2) {
		klog("ACPI v2.0 or above\n");

		if (rsdp->xsdt_addr) {
			acpi_xsdt_parse((xsdt_t *)rsdp->xsdt_addr);
		} else {
			acpi_rsdt_parse((rsdt_t *)rsdp->rsdt_addr);
		}
	} else {
		klog("ACPI v1.0\n");
		acpi_rsdt_parse((rsdt_t *)rsdp->rsdt_addr);
	}
}

void acpi_rsdt_parse(rsdt_t *rsdt)
{
	int entries = (rsdt->header.len - sizeof(rsdt->header)) / 4;

	for (int i = 0; i < entries; i++) {
		sdt_t *header = (sdt_t *)rsdt->ptr[i];

		// TODO: Add some other tables in the future
		// But MADT (sig: APIC) is enough for now
		if (strncmp(header->sig, "APIC", 4) == 0) {
			acpi_madt_init((madt_t *)rsdt->ptr[i]);
		}
	}
}

void acpi_xsdt_parse(xsdt_t *xsdt)
{
	int entries = (xsdt->header.len - sizeof(xsdt->header)) / 4;

	for (int i = 0; i < entries; i++) {
		sdt_t *header = (sdt_t *)xsdt->ptr[i];

		// TODO: Add some other tables in the future
		// But MADT (sig: APIC) is enough for now
		if (strncmp(header->sig, "APIC", 4) == 0) {
			acpi_madt_init((madt_t *)xsdt->ptr[i]);
		}
	}
}