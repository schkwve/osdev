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

#ifndef _GDT_H_
#define _GDT_H_

typedef struct {
	uint16_t limit_lo;
	uint16_t base_lo;
	uint8_t base_mid;
	uint8_t access;
	uint8_t limit_hi : 4;
	uint8_t flags : 4;
	uint8_t base_hi;
} gdt_entry_t;

typedef struct {
	uint16_t size;
	uint32_t offset;
} gdtr_t;

void gdt_init();
void gdt_set_desc(gdt_entry_t *gdt, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

extern void gdt_load(gdtr_t *gdtr);
extern void gdt_reloadseg(void);

#endif // _GDT_H_