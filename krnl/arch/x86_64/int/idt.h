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

#ifndef __IDT_H_
#define __IDT_H_

#include <arch.h>

typedef struct {
	uint16_t base_lo;
	uint16_t cs;
	uint8_t ist;
	uint8_t flags;
	uint16_t base_mid;
	uint32_t base_hi;
	uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed)) idtr_t;

void idt_init();
void idt_set_desc(uint8_t vector, void *isr, uint8_t flags, uint8_t ist);

extern void idt_load(void);

#endif // __IDT_H_