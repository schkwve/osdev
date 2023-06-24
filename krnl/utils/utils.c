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

#include <mm/vmm.h>

#include <utils/utils.h>
#include <stdint.h>

void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx,
		   uint32_t *edx)
{
	__asm__ volatile("cpuid"
					 : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
					 : "0"(reg));
}

uint64_t rdmsr(uint32_t msr)
{
	uint64_t rax;
	uint64_t rdx;
	__asm__ volatile("rdmsr" : "=a"(rax), "=d"(rdx) : "c"(msr) : "memory");
	return (rdx << 32) | rax;
}

void wrmsr(uint32_t msr, uint64_t val)
{
	uint64_t rax = (uint32_t)val;
	uint64_t rdx = val >> 32;
	__asm__ volatile("wrmsr" ::"a"(rax), "d"(rdx), "c"(msr));
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

uint64_t read_cr0(void)
{
	uint64_t cr0;
	__asm__ volatile("mov %%cr0, %0" : "=rax"(cr0));
	return cr0;
}

uint64_t read_cr2(void)
{
	uint64_t cr2;
	__asm__ volatile("mov %%cr2, %0" : "=rax"(cr2));
	return cr2;
}

uint64_t read_cr3(void)
{
	uint64_t cr3;
	__asm__ volatile("mov %%cr0, %0" : "=rax"(cr3));
	return cr3;
}

uint64_t read_cr4(void)
{
	uint64_t cr4;
	__asm__ volatile("mov %%cr0, %0" : "=rax"(cr4));
	return cr4;
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

void enable_pat(void)
{
	wrmsr(0x277, PAT_UNCACHEABLE | (PAT_WRITE_COMBINING << 8) | (PAT_WRITE_THRU << 32) | (PAT_WRITE_PROT << 40) | (PAT_WRITE_BACK << 48) | (PAT_UNCACHED << 56));
}

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