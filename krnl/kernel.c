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

#include <acpi/acpi.h>

#include <boot/limine.h>

#include <cpu/cpu.h>
#include <cpu/gdt.h>

#include <int/idt.h>
#include <int/isr.h>
#include <int/irq.h>

#include <mm/phys.h>
#include <mm/virt.h>

#include <dd/apic/apic.h>
#include <dd/apic/pic.h>
#include <dd/pit/pit.h>
#include <dd/ps2/kbd.h>
#include <dd/serial/serial.h>

#include <debug/log.h>

static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
};

static volatile struct limine_rsdp_request rsdp_request = {
	.id = LIMINE_RSDP_REQUEST,
	.revision = 0,
};

static volatile struct limine_kernel_address_request kernel_addr_request = {
	.id = LIMINE_KERNEL_ADDRESS_REQUEST,
	.revision = 0,
};

void _start(void)
{
	serial_init();

	gdt_init();
	idt_init();

	acpi_init(rsdp_request.response);

	//apic_init();
	phys_init(memmap_request.response);
	virt_init(memmap_request.response, kernel_addr_request.response);
	cpu_check();

	pit_init();

	ps2_kbd_init();

	klog("Init done\n");

	klog("\nGoodbye, friend.\n");

	for (;;) {
	}
}