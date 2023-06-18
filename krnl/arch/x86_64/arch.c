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

#ifdef x86_64

#include <arch.h>
#include <debug/log.h>

extern uint64_t ioapic_ptr;

void arch_init()
{
	serial_init();

	gdt_init();
	idt_init();
	irq_init();

	acpi_init();
	cpu_check();

	lapic_init();
	ioapic_init();

	pit_init();
	ioapic_set_entry(ioapic_ptr, acpi_remap_irq(IRQ_PIT), INT_PIT);
	sti();
	
	klog("Init done\n");
}

#endif