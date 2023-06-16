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

typedef struct {
	rsdp_t rsdp;
	uint32_t len;
	uint64_t xsdt_addr;
	uint8_t ext_cs;
	uint8_t reserved[3];
} __attribute__((packed)) rsdp20_t;

///

typedef struct {
	char sig[4];
	uint32_t len;
	uint8_t rev;
	uint8_t cs;
	char oem_id[6];
	char oem_tbl_id[8];
	uint32_t oem_rev;
	uint32_t creator_id;
	uint32_t creator_rev;
} rsdt_t;

void acpi_rsdp_init();

void acpi_rsdt_init(rsdt_t *rsdt);
//void acpi_xsdt_init(xsdt_t *xsdt_addr);

#endif // __RSDT_H_