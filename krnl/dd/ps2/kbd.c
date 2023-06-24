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

#include <dd/apic/pic.h>
#include <int/irq.h>

#include <utils/utils.h>

#include "kbd.h"

void ps2_kbd_init()
{
	pic_unmask(1);
	irq_register(1, ps2_kbd_handler);

	while (inb(PS2_KBD_CMD) & 0x1) {
		inb(PS2_KBD_DATA);
	}

	ps2_kbd_send_cmd(0xF4);
}

void ps2_kbd_send_cmd(uint8_t cmd)
{
	while (inb(PS2_KBD_CMD) & 0x2) {
		outb(PS2_KBD_DATA, cmd);
	}
}

void ps2_kbd_handler(cpu_regs_t *regs)
{
	uint8_t scancode = inb(PS2_KBD_DATA);
	klog("0x%x ", scancode);
}