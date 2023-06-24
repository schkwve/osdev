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

#ifndef __GDT_H_
#define __GDT_H_

#include <stdint.h>

typedef struct {
	uint16_t limit;
	uint16_t base_lo;
	uint8_t base_mid;
	uint8_t type;
	uint8_t flags;
	uint8_t base_hi;
} __attribute__((packed)) gdt_desc_t;

typedef struct {
	uint32_t reserved0;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t reserved1;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t reserved2;
	uint16_t reserved3;
	uint16_t iopb_offset;
} __attribute__((packed)) tss_t;

typedef struct gdt_t {
	gdt_desc_t entry[10];
} __attribute__((packed)) __attribute__((aligned(0x1000))) gdt_t;

typedef struct {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed)) gdtr_t;

void gdt_init(void);
void gdt_set_entry(int entry, uint16_t limit, uint32_t base, uint8_t type,
				   uint8_t flags);

extern void gdt_load(gdtr_t *gdtr);

#endif // __GDT_H_