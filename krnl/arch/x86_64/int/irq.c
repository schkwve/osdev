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

irq_handler_t __handlers[16];

void irq_init(void)
{
	pic_init(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

	for (int i = 0; i < 16; i++) {
		isr_register(PIC_REMAP_OFFSET + i, irq_handler);
	}
}

void irq_register(int irq, irq_handler_t handler)
{
	__handlers[irq] = handler;
	klog("Registered IRQ%i\n", irq);
}

void irq_deregister(int irq)
{
	__handlers[irq] = NULL;
	klog("Deregistered IRQ%i\n", irq);
}

void irq_handler(cpu_regs_t *regs)
{
	int irq = regs->int_no - PIC_REMAP_OFFSET;

	if (__handlers[irq] != NULL) {
		__handlers[irq](regs);
	} else {
		klog("Unhandled IRQ%i\n", irq);
	}

	pic_eoi(irq);
}