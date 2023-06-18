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
#include <debug/log.h>

#include <stdint.h>

void cpu_check()
{
	uint32_t eax, ebx, ecx, edx;

	uint32_t lsf;
	char cpu_vendor[13];
	cpuid(0, &lsf, (uint32_t *)cpu_vendor, (uint32_t *)(cpu_vendor + 8),
		  (uint32_t *)(cpu_vendor + 4));
	cpu_vendor[12] = '\0';

	if (lsf >= 0x01) {
		cpuid(0x01, &eax, &ebx, &ecx, &edx);

		if (!(edx & EDX_APIC)) {
			//panic("CPU does not have APIC!");
			klog("CPU does not have APIC! Halting...\n");
			for (;;) {
				__asm__ volatile("cli;hlt");
			}
		}
		if (!(edx & EDX_MSR)) {
			//panic("CPU does not have MSR!");
			klog("CPU does not have MSR! Halting...\n");
			for (;;) {
				__asm__ volatile("cli;hlt");
			}
		}
	}

	uint32_t lef;
	char cpu_name[48];
	const char *p;
	cpuid(0x80000000, &lef, &ebx, &ecx, &edx);

	if (lef >= 0x80000004) {
		cpuid(0x80000002, (uint32_t *)(cpu_name), (uint32_t *)(cpu_name + 4),
			  (uint32_t *)(cpu_name + 8), (uint32_t *)(cpu_name + 12));
		cpuid(0x80000003, (uint32_t *)(cpu_name + 16),
			  (uint32_t *)(cpu_name + 20), (uint32_t *)(cpu_name + 24),
			  (uint32_t *)(cpu_name + 28));
		cpuid(0x80000004, (uint32_t *)(cpu_name + 32),
			  (uint32_t *)(cpu_name + 36), (uint32_t *)(cpu_name + 40),
			  (uint32_t *)(cpu_name + 44));

		p = cpu_name;
		while (*p == ' ')
			++p;
	}

	klog("CPU Vendor: %s\n", cpu_vendor);
	klog("CPU Name: %s\n", p);
}