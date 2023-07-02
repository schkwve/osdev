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

#ifndef __APIC_H_
#define __APIC_H_

#include <stdint.h>

typedef struct {
	uint8_t type;
	uint8_t len;
} __attribute__((packed)) apic_hdr_t;

typedef struct {
	apic_hdr_t hdr;
	uint8_t proc_id;
	uint8_t lapic_id;
	uint32_t flags;
} apic_lapic_t;

typedef struct {
	apic_hdr_t hdr;
	uint8_t ioapic_id;
	uint8_t reserved;
	uint32_t ioapic_addr;
	uint32_t gsi_base;
} apic_ioapic_t;

typedef struct {
	apic_hdr_t hdr;
	uint8_t bus_src;
	uint8_t irq_src;
	uint32_t gsi;
	uint16_t flags;
} apic_iso_t;

extern uint8_t *g_ioapic_addr;

void apic_init(void);

#endif // __APIC_H_