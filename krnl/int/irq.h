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

#ifndef __IRQ_H_
#define __IRQ_H_

#include <cpu/cpu.h>

#define IRQ_TIMER 0
#define IRQ_PS2_KBD 1
#define IRQ_COM2 3
#define IRQ_COM1 4
#define IRQ_FLOPPY 6
#define IRQ_CMOS 8
#define IRQ_PS2_MOUSE 12
#define IRQ_ATA0 14
#define IRQ_ATA1 15

#define INT_TIMER IRQ_TIMER + 0x20
#define INT_PS2_KBD IRQ_PS2_KBD + 0x20
#define INT_COM2 IRQ_COM2 + 0x20
#define INT_COM1 IRQ_COM1 + 0x20
#define INT_FLOPPY IRQ_FLOPPY + 0x20
#define INT_CMOS IRQ_CMOS + 0x20
#define INT_PS2_MOUSE IRQ_PS2_MOUSE + 0x20
#define INT_ATA0 IRQ_ATA0 + 0x20
#define INT_ATA1 IRQ_ATA1 + 0x20

typedef void (*irq_handler_t)(cpu_regs_t *regs);

void irq_register(int irq, irq_handler_t handler);
void irq_deregister(int irq);

#endif // __IRQ_H_