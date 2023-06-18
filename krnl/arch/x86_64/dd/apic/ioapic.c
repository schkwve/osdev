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

extern uint64_t ioapic_ptr;

void ioapic_init()
{
}

uint32_t ioapic_in(uint8_t *base, uint8_t reg)
{
	mmio_outw(base + IOREGSEL, reg);
	return mmio_inw(base + IOWIN);
}

void ioapic_out(uint8_t *base, uint8_t reg, uint32_t val)
{
	mmio_outw(base + IOREGSEL, reg);
	mmio_outw(base + IOWIN, val);
}