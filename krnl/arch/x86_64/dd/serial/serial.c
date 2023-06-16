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

void serial_init()
{
	outb(COM1 + 1, 0x00);
	outb(COM1 + 3, 0x80);
	outb(COM1 + 0, 0x03); // Set divisor to 38400 baud (low byte)
	outb(COM1 + 1, 0x00); // (high byte)
	outb(COM1 + 3, 0x03);
	outb(COM1 + 2, 0xC7);
	outb(COM1 + 4, 0x0B);
}

int serial_received()
{
	return inb(COM1 + 5) & 1;
}

char serial_read()
{
	while (serial_received() == 0)
		;
	return inb(COM1);
}

int serial_transmit_empty()
{
	return inb(COM1 + 5) & 0x20;
}

void serial_putc(char c)
{
	while (serial_transmit_empty() == 0)
		;
	outb(COM1, c);
}

void serial_write(char *str)
{
	while (*str) {
		serial_putc(*str++);
	}
}