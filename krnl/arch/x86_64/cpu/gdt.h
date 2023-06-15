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
	uint16_t limit;
	uint16_t base_lo;
	uint8_t base_mid;
	uint8_t access;
	uint8_t flags;
	uint8_t base_hi;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
	uint16_t size;
	uint16_t base_lo;
	uint8_t base_mid;
	uint8_t flags1;
	uint8_t flags2;
	uint8_t base_hi;
	uint32_t base_upper;
	uint32_t reserved;
} __attribute__((packed)) tss_entry_t;

typedef struct {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed)) gdtptr_t;

typedef struct {
	gdt_entry_t entry[5];
	tss_entry_t tss;
} __attribute__((packed)) __attribute__((aligned(0x1000))) gdtr_t;

typedef struct {
	uint32_t reserved;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t reserved2;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t reserved3;
	uint16_t reserved4;
	uint16_t iomap_base;
} __attribute__((packed)) tss_t;

void gdt_init();
void gdt_set_entry(int entry, uint16_t limit, uint32_t base, uint8_t access,
				   uint8_t flags);

extern void gdt_load(void);
extern void tss_load(void);

#endif // _GDT_H_