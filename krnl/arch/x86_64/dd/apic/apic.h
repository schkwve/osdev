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

#define APIC_BASE_MSR 0x1B
#define APIC_BASE_MSR_BSP 0x100
#define APIC_BASE_MSR_ENABLE 0x800

#define LAPIC_ID 0x20 // LAPIC ID
#define LAPIC_TPR 0x80 // Task Priority
#define LAPIC_LDR 0xd0 // Logical Destination
#define LAPIC_DFR 0xe0 // Destination Format
#define LAPIC_SVR 0xf0 // Spurious Interrupt Vector
#define LAPIC_ICRLO 0x0300 // Interrupt Command
#define LAPIC_ICRHI 0x0310 // Interrupt Command

#define ICR_INIT 0x00000500
#define ICR_STARTUP 0x00000600
#define ICR_PHYSICAL 0x00000000
#define ICR_SEND_PENDING 0x00001000
#define ICR_ASSERT 0x00004000
#define ICR_EDGE 0x00000000
#define ICR_NO_SHORTHAND 0x00000000
#define ICR_DESTINATION_SHIFT 24 // Destination Field

typedef struct {
	uint8_t type;
	uint8_t len;
} __attribute__((packed)) apic_hdr_t;

typedef struct {
	apic_hdr_t hdr;
	uint8_t bus;
	uint8_t src;
	uint32_t interrupt;
	uint16_t flags;
} __attribute__((packed)) apic_int_override_t;

void apic_init();

uint32_t lapic_get_id();

void lapic_send_init(uint32_t apic_id);
void lapic_send_startup(uint32_t apic_id, uint32_t vector);

uint32_t lapic_in(uint32_t reg);
void lapic_out(uint32_t reg, uint32_t data);

#endif // __APIC_H_