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

void rdmsr(uint32_t msr, uint32_t *low, uint32_t *high);
void wrmsr(uint32_t msr, uint32_t low, uint32_t high);

void outb(uint16_t port, uint8_t str);
uint8_t inb(uint16_t port);

void sti(void);
void cli(void);

///

void io_wait(void);

void mmio_outb(void *p, uint8_t data);
uint8_t mmio_inb(void *p);
void mmio_outc(void *p, uint16_t data);
uint16_t mmio_inc(void *p);
void mmio_outw(void *p, uint32_t data);
uint32_t mmio_inw(void *p);
void mmio_outq(void *p, uint64_t data);
uint64_t mmio_inq(void *p);

#endif // __UTILS_H_