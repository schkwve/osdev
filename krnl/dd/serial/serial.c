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

#include <dd/serial/serial.h>
#include <utils.h>

void serial_init()
{
	outb(COM1 + 1, 0x00);        /* Disable all interrupts */
    outb(COM1 + 3, 0x80);        /* Enable DLAB (set baud rate divisor) */
    outb(COM1 + 0, 0x03);        /* Set divisor to 3 (lo byte) 38400 baud */
    outb(COM1 + 1, 0x00);        /* (hi byte) */
    outb(COM1 + 3, 0x03);        /* 8 bits, no parity, one stop bit */
    outb(COM1 + 2, 0xC7);        /* Enable FIFO, clear them, with 14-byte */
    outb(COM1 + 4, 0x0B);        /* IRQs enabled, RTS/DSR set */
}

int serial_received()
{
	return inb(COM1 + 5) & 1;
}

char serial_read()
{
	while (serial_received() == 0);
	return inb(COM1);
}

int serial_transmit_empty()
{
	return inb(COM1 + 5) & 0x20;
}

void serial_write(char c)
{
	while (serial_transmit_empty() == 0);
	outb(COM1, c);
}