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

#ifndef __MADT_H_
#define __MADT_H_

#include <acpi/sdt.h>

#include <dd/apic/apic.h>
#include <dd/apic/lapic.h>

#include <stdint.h>

#define ACPI_MADT_LAPIC 0
#define ACPI_MADT_IOAPIC 1
#define ACPI_MADT_ISO 2

typedef struct {
	sdt_t hdr;
	uint32_t lapic_addr;
	uint32_t flags;
} madt_t;

void acpi_madt_init(madt_t *_madt);

extern apic_iso_t *g_apic_iso[16];
extern uint8_t g_apic_isos;

extern madt_t *g_madt;

#endif // __MADT_H_