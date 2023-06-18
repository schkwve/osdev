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

#include <arch.h>

#include <stdint.h>

void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx,
		   uint32_t *edx)
{
	__asm__ volatile("cpuid"
					 : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
					 : "0"(reg));
}

void rdmsr(uint32_t msr, uint32_t *low, uint32_t *high)
{
	__asm__ volatile("rdmsr" : "=a"(*low), "=d"(*high) : "c"(msr));
}

void wrmsr(uint32_t msr, uint32_t low, uint32_t high)
{
	__asm__ volatile("wrmsr" ::"a"(low), "d"(high), "c"(msr));
}

void outb(uint16_t port, uint8_t str)
{
	__asm__ volatile("outb %0, %1" ::"a"(str), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

void sti(void)
{
	__asm__ volatile("sti");
}

void cli(void)
{
	__asm__ volatile("cli");
}

///

void io_wait(void)
{
	outb(0x80, 0);
}

void mmio_outw(void *p, uint32_t data)
{
	*(volatile uint32_t *)(p) = data;
}

uint32_t mmio_inw(void *p)
{
	return *(volatile uint32_t *)(p);
}