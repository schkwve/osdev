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

#ifndef __ARCH_H_
#define __ARCH_H_

#ifdef x86_64
#include <x86/utils.h>

#include <x86_64/acpi/acpi.h>
#include <x86_64/acpi/rsdt.h>
#include <x86_64/acpi/madt.h>
#include <x86_64/acpi/sdt.h>

#include <x86_64/boot/limine.h>

#include <x86_64/cpu/cpu.h>
#include <x86_64/cpu/gdt.h>

#include <x86_64/dd/serial/serial.h>
#include <x86_64/dd/apic/pic.h>
#include <x86_64/dd/apic/apic.h>
#include <x86_64/dd/pit/pit.h>

#include <x86_64/int/idt.h>
#include <x86_64/int/isr.h>
#include <x86_64/int/irq.h>
#else
#error Unsupported architecture!
#endif

void arch_init();

#endif // __ARCH_H_