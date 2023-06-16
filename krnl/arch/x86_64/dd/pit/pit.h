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

#ifndef __PIT_H_
#define __PIT_H_

#define PIT_COUNTER0 0x40
#define PIT_CMD 0x43

#define PIT_BINARY 0x00
#define PIT_MODE3 0x04
#define PIT_RW_BOTH 0x30

void pit_init();
void pit_handler();
uint8_t pit_get_ticks();
void pit_wait(uint8_t ms);

#endif // __PIT_H_