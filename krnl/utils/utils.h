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

#ifndef __UTILS_H_
#define __UTILS_H_

#include <stdint.h>

void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx,
		   uint32_t *edx);

uint64_t rdmsr(uint32_t msr);
void wrmsr(uint32_t msr, uint64_t val);

void outb(uint16_t port, uint8_t str);
uint8_t inb(uint16_t port);

uint64_t read_cr0(void);
uint64_t read_cr2(void);
uint64_t read_cr3(void);
uint64_t read_cr4(void);

void sti(void);
void cli(void);

///

void enable_pat(void);

void io_wait(void);

void mmio_outw(void *p, uint32_t data);
uint32_t mmio_inw(void *p);

#endif // __UTILS_H_