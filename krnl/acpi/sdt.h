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

#ifndef __SDT_H_
#define __SDT_H_

#include <stdint.h>

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
} __attribute__((packed)) sdt_t;

#endif // __SDT_H_