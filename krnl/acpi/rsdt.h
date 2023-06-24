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

#ifndef __RSDT_H_
#define __RSDT_H_

#include <acpi/sdt.h>

#include <stdint.h>

typedef struct {
	char sig[8];
	uint8_t cs;
	char oem_id[6];
	uint8_t rev;
	uint32_t rsdt_addr;
	uint32_t len;
	uint64_t xsdt_addr;
	uint8_t ext_cs;
	uint8_t reserved[3];
} __attribute__((packed)) rsdp_t;

///

typedef struct {
	sdt_t header;
	uint32_t ptr[];
} __attribute__((packed)) rsdt_t;

typedef struct {
	sdt_t header;
	uint64_t ptr[];
} __attribute__((packed)) xsdt_t;

uint64_t acpi_rsdp_init();

#endif // __RSDT_H_