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
#include <stdint.h>

extern uint8_t lapic_ids[256];
extern uint8_t ncores;
extern uint64_t lapic_ptr;
extern uint64_t ioapic_ptr;

void lapic_init()
{
	pic_disable();

	// enable all interrupts by clearing task priority
	lapic_out(LAPIC_TPR, 0);

	// flat mode
	lapic_out(LAPIC_DFR, 0xFFFFFFFF);
	// logical ID 1
	lapic_out(LAPIC_LDR, 0x01000000);

	// spurious interrupt vector register
	lapic_out(LAPIC_SVR, 0x100 | 0xFF);
}

// For later SMP support
void lapic_send_init(uint32_t apic_id)
{
	lapic_out(LAPIC_ICRHI, apic_id << ICR_DESTINATION_SHIFT);
	lapic_out(LAPIC_ICRLO, ICR_INIT | ICR_PHYSICAL | ICR_ASSERT | ICR_EDGE |
							   ICR_NO_SHORTHAND);

	while (lapic_in(LAPIC_ICRLO) & ICR_SEND_PENDING)
		;
}

void lapic_send_startup(uint32_t apic_id, uint32_t vector)
{
	lapic_out(LAPIC_ICRHI, apic_id << ICR_DESTINATION_SHIFT);
	lapic_out(LAPIC_ICRLO, vector | ICR_STARTUP | ICR_PHYSICAL | ICR_ASSERT |
							   ICR_EDGE | ICR_NO_SHORTHAND);

	while (lapic_in(LAPIC_ICRLO) & ICR_SEND_PENDING)
		;
}

uint32_t lapic_in(uint32_t reg)
{
	return mmio_inw(lapic_ptr + reg);
}

void lapic_out(uint32_t reg, uint32_t data)
{
	mmio_outw(lapic_ptr + reg, data);
}