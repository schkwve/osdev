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

#ifndef __IOAPIC_H_
#define __IOAPIC_H_

#include <stdint.h>

#define IOREGSEL 0x00
#define IOWIN 0x10

#define IOAPICVER 0x01
#define IOREDTBL 0x10

void ioapic_init();

void ioapic_set_entry(uint8_t *base, uint8_t index, uint64_t data);

uint32_t ioapic_in(uint8_t *base, uint8_t reg);
void ioapic_out(uint8_t *base, uint8_t reg, uint32_t val);

#endif // __IOAPIC_H_