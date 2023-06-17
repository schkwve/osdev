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

#include <debug/log.h>

uint8_t pit_ticks;

void pit_init()
{
	uint16_t divisor = 1193182 / 1000;
	outb(PIT_CMD, PIT_BINARY | PIT_MODE3 | PIT_RW_BOTH | PIT_COUNTER0);
	outb(PIT_COUNTER0, divisor);
	outb(PIT_COUNTER0, divisor >> 8);

	irq_register(0, &pit_handler);
}

void pit_handler()
{
	pit_ticks += 1;
	pic_eoi(0);
}

uint8_t pit_get_ticks()
{
	return pit_ticks;
}

void pit_wait(uint8_t ms)
{
	uint8_t now = pit_get_ticks();
	++ms;

	while (pit_get_ticks() - now < ms)
		;
}