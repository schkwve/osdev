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

#ifndef __PS2_KBD_H_
#define __PS2_KBD_H_

#include <cpu/cpu.h>

#include <stdint.h>

#define PS2_KBD_CMD 0x64
#define PS2_KBD_DATA 0x60

void ps2_kbd_init(void);
void ps2_kbd_send_cmd(uint8_t cmd);

uint8_t ps2_kbd_to_ascii(uint8_t key);
void ps2_kbd_handler(cpu_regs_t *regs);

#endif // __PS2_KBD_H_