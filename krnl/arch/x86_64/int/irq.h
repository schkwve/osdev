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

#define IRQ_PIT 0x00
#define IRQ_KBD 0x01
#define IRQ_COM2 0x03
#define IRQ_COM1 0x04
#define IRQ_FLOPPY 0x06
#define IRQ_ATA0 0x0E
#define IRQ_ATA1 0x0F

#define INT_PIT 0x20
#define INT_SPU 0xFF

typedef void (*irq_handler_t)(cpu_regs_t *regs);

void irq_init(void);

void irq_register(int irq, irq_handler_t handler);
void irq_deregister(int irq);

void irq_handler(cpu_regs_t *regs);

#endif // __IRQ_H_