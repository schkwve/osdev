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

#include <debug/log.h>
#include <utils/utils.h>

#include "kbd.h"

enum KEYCODE {
	NULL_KEY = 0,
	Q_PRESSED = 0x10,
	Q_RELEASED = 0x90,
	W_PRESSED = 0x11,
	W_RELEASED = 0x91,
	E_PRESSED = 0x12,
	E_RELEASED = 0x92,
	R_PRESSED = 0x13,
	R_RELEASED = 0x93,
	T_PRESSED = 0x14,
	T_RELEASED = 0x94,
	Z_PRESSED = 0x15,
	Z_RELEASED = 0x95,
	U_PRESSED = 0x16,
	U_RELEASED = 0x96,
	I_PRESSED = 0x17,
	I_RELEASED = 0x97,
	O_PRESSED = 0x18,
	O_RELEASED = 0x98,
	P_PRESSED = 0x19,
	P_RELEASED = 0x99,
	A_PRESSED = 0x1E,
	A_RELEASED = 0x9E,
	S_PRESSED = 0x1F,
	S_RELEASED = 0x9F,
	D_PRESSED = 0x20,
	D_RELEASED = 0xA0,
	F_PRESSED = 0x21,
	F_RELEASED = 0xA1,
	G_PRESSED = 0x22,
	G_RELEASED = 0xA2,
	H_PRESSED = 0x23,
	H_RELEASED = 0xA3,
	J_PRESSED = 0x24,
	J_RELEASED = 0xA4,
	K_PRESSED = 0x25,
	K_RELEASED = 0xA5,
	L_PRESSED = 0x26,
	L_RELEASED = 0xA6,
	Y_PRESSED = 0x2C,
	Y_RELEASED = 0xAC,
	X_PRESSED = 0x2D,
	X_RELEASED = 0xAD,
	C_PRESSED = 0x2E,
	C_RELEASED = 0xAE,
	V_PRESSED = 0x2F,
	V_RELEASED = 0xAF,
	B_PRESSED = 0x30,
	B_RELEASED = 0xB0,
	N_PRESSED = 0x31,
	N_RELEASED = 0xB1,
	M_PRESSED = 0x32,
	M_RELEASED = 0xB2,

	ZERO_PRESSED = 0x29,
	ONE_PRESSED = 0x2,
	NINE_PRESSED = 0xA,

	POINT_PRESSED = 0x34,
	POINT_RELEASED = 0xB4,

	SLASH_RELEASED = 0xB5,

	BACKSPACE_PRESSED = 0xE,
	BACKSPACE_RELEASED = 0x8E,
	SPACE_PRESSED = 0x39,
	SPACE_RELEASED = 0xB9,
	ENTER_PRESSED = 0x1C,
	ENTER_RELEASED = 0x9C,
};

static char* _qwertzuiop = "qwertzuiop"; // 0x10-0x1c
static char* _asdfghjkl = "asdfghjkl";
static char* _yxcvbnm = "yxcvbnm";
static char* _num = "123456789";
uint8_t ps2_kbd_to_ascii(uint8_t key)
{
	//kprintf("key=0x%x\n", key);
	if(key == 0x1C) return '\n';
	if(key == 0x39) return ' ';
	if(key == 0xE) return '\r';
	if(key == POINT_RELEASED) return '.';
	if(key == SLASH_RELEASED) return '/';
	if(key == ZERO_PRESSED) return '0';
	if(key >= ONE_PRESSED && key <= NINE_PRESSED)
		return _num[key - ONE_PRESSED];
	if(key >= 0x10 && key <= 0x1C)
	{
		return _qwertzuiop[key - 0x10];
	} else if(key >= 0x1E && key <= 0x26)
	{
		return _asdfghjkl[key - 0x1E];
	} else if(key >= 0x2C && key <= 0x32)
	{
		return _yxcvbnm[key - 0x2C];
	}
	return 0;
}


void ps2_kbd_init()
{
	pic_unmask(1);
	irq_register(1, ps2_kbd_handler);

	while (inb(PS2_KBD_CMD) & 0x1) {
		inb(PS2_KBD_DATA);
	}

	ps2_kbd_send_cmd(0xF4);
	klog("done\n");
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
}