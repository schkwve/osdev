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

static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
};

void acpi_rsdp_init()
{
	uint64_t *rsdp_addr = rsdp_request.response->address;
	
	// Validate RSDP
	uint8_t checksum = 0;

	for (uint8_t i = 0; i < 20; i++) {
		checksum += ((uint8_t *)rsdp_addr)[i];
	}

	if ((checksum & 0xFF) != 0) {
		klog("Invalid RSDP Checksum!\n");
	}

	rsdp_t *rsdp = (rsdp_t *)rsdp_addr;
	if (rsdp->rev >= 2) {
		// Not tested, but should work
		klog("ACPI v2.0 or above\n");
		rsdt_t *xsdt = (rsdt_t *)rsdp->xsdt_addr;

		uint8_t checksum = 0;
		for (uint8_t i = 0; i <= xsdt->len; i++) {
			checksum += ((uint8_t *)xsdt)[i];
		}

		if (checksum != 0) {
			klog("Invalid XSDT Checksum\n");
		}
	} else {
		klog("ACPI v1.0\n");
		rsdt_t *rsdt = (rsdt_t *)rsdp->rsdt_addr;

		uint8_t checksum = 0;
		for (uint8_t i = 0; i <= rsdt->len; i++) {
			checksum += ((uint8_t *)rsdt)[i];
		}

		if (checksum != 0) {
			klog("Invalid RSDT Checksum\n");
		}
	}
}

void acpi_rsdt_init(rsdt_t *rsdt)
{
	// Validate RSDT
	uint8_t checksum = 0;
	for (uint8_t i = 0; i <= rsdt->len; i++) {
		checksum += ((uint8_t *)rsdt)[i];
	}

	if (checksum != 0) {
		klog("Invalid RSDT Checksum\n");
	}
}

