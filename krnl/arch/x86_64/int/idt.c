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

__attribute__((aligned(0x10)))
idt_entry_t idt[256];
idtr_t idtr;

isr_handler_t __handlers[256];
extern uint64_t *isr_tbl[];

void idt_init()
{
	idtr.offset = (uint64_t)&idt[0];
	idtr.size = (uint16_t)sizeof(idt_entry_t) * 256 - 1;

	for (uint8_t vector = 0; vector < 32; vector++) {
		idt_set_desc(vector, isr_tbl[vector], 0x8E, 0);
	}

	idt_load();
	klog("Loaded IDT\n");
}

void idt_set_desc(uint8_t vector, void *isr, uint8_t flags, uint8_t ist)
{
	idt_entry_t *desc = &idt[vector];

	desc->base_lo = (uint64_t)isr & 0xFFFF;
	desc->cs = 0x08;
	desc->ist = ist;
	desc->flags = flags;
	desc->base_mid = ((uint64_t)isr >> 16) & 0xFFFF;
	desc->base_hi = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
	desc->reserved = 0;
}

void idt_register(uint8_t vector, isr_handler_t handler)
{
	__handlers[vector] = handler;
	idt_set_desc(vector, isr_tbl[vector], 0x8E, 2);
	klog("Registed INT#%i\n", vector);
}

void idt_deregister(uint8_t vector)
{
	// TODO: NULL
	idt_set_desc(vector, 0, 0, 0);
	__handlers[vector] = NULL;
	klog("Deregisted INT#%i\n", vector);
}