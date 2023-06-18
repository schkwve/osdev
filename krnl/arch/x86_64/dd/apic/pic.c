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

void pic_init(uint8_t offset1, uint8_t offset2)
{
	uint8_t mask1 = inb(PIC1_DATA);
	uint8_t mask2 = inb(PIC2_DATA);

	pic_send(PIC1_CMD, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
	pic_send(PIC2_CMD, PIC_ICW1_ICW4 | PIC_ICW1_INIT);

	pic_send(PIC1_DATA, offset1);
	pic_send(PIC2_DATA, offset2);

	pic_send(PIC1_DATA, 0x4);
	pic_send(PIC2_DATA, 0x2);

	pic_send(PIC1_DATA, PIC_ICW4_8086);
	pic_send(PIC2_DATA, PIC_ICW4_8086);

	// restore masks
	pic_send(PIC1_DATA, mask1);
	pic_send(PIC2_DATA, mask2);
}

void pic_eoi(int irq)
{
	if (irq >= 8) {
		pic_send(PIC2_CMD, PIC_EOI);
	}

	pic_send(PIC1_CMD, PIC_EOI);
}

uint16_t pic_get_irr()
{
	pic_send(PIC1_CMD, PIC_READ_IRR);
	pic_send(PIC2_CMD, PIC_READ_IRR);

	return (inb(PIC2_CMD) | (inb(PIC2_CMD) << 8));
}

uint16_t pic_get_isr()
{
	pic_send(PIC1_CMD, PIC_READ_ISR);
	pic_send(PIC2_CMD, PIC_READ_ISR);

	return (inb(PIC2_CMD) | (inb(PIC2_CMD) << 8));
}

void pic_mask(int irq)
{
	uint8_t port;

	if (irq < 8) {
		port = PIC1_DATA;
	} else {
		irq -= 8;
		port = PIC2_DATA;
	}

	uint8_t mask = inb(port);
	pic_send(port, mask | (1 << irq));
}

void pic_unmask(int irq)
{
	uint8_t port;

	if (irq < 8) {
		port = PIC1_DATA;
	} else {
		irq -= 8;
		port = PIC2_DATA;
	}

	uint8_t mask = inb(port);
	pic_send(port, mask & ~(1 << irq));
}

void pic_disable()
{
	pic_send(PIC1_DATA, 0xFF);
	pic_send(PIC2_DATA, 0xFF);
}

void pic_send(uint16_t pic, uint8_t data)
{
	outb(pic, data);
	io_wait();
}