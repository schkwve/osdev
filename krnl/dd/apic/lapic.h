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

#ifndef __LAPIC_H_
#define __LAPIC_H_

#include <stdint.h>

#define LAPIC_SPURIOUS 0x0F0
#define LAPIC_SPURIOUS_INT 255
#define LAPIC_ENABLE (1 << 8)
#define LAPIC_EOI 0xB0

void lapic_init(void);

void lapic_eoi(void);
void lapic_out(uint32_t reg, uint32_t data);
void lapic_in(uint32_t reg);

#endif // __LAPIC_H_