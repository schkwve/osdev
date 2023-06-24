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

#include <int/idt.h>
#include <dd/apic/pic.h>
#include <debug/log.h>

#include <stdint.h>

static idt_desc_t idt[256];
static idtr_t idtr;

extern uintptr_t isr_tbl[];
extern void *irq_handlers[16];

void idt_init(void)
{
	for (int i = 0; i < 48; i++) {
		idt_set_desc(i, INT_GATE);
	}

	// spurious interrupt
	idt_set_desc(255, INT_GATE);

	pic_init(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 0x08);

	for (int i = 0; i < 16; i++) {
		pic_mask(i);
		irq_handlers[i] = NULL;
	}

	idtr.size = sizeof(idt) - 1;
	idtr.offset = (uint64_t)&idt;

	idt_load(&idtr);
	klog("IDT init\n");
}

void idt_set_desc(uint8_t index, uint8_t flags)
{
	uint64_t base = isr_tbl[index];

	idt[index].base_lo = base & 0xFFFF;
	idt[index].cs = 0x08;
	idt[index].ist = 0;
	idt[index].flags = flags;
	idt[index].base_mid = (base >> 16) & 0xFFFF;
	idt[index].base_hi = (base >> 32) & 0xFFFFFFFF;
	idt[index].reserved = 0;
}