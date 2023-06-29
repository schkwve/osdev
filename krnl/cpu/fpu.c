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

#include <cpu/fpu.h>

#include <debug/log.h>
#include <utils/utils.h>

void fpu_init(void)
{
	uint64_t cr0 = read_cr0();
	cr0 |= (1 << 1);
	cr0 &= ~(1 << 2);
	cr0 &= ~(1 << 5);
	write_cr0(cr0);
	__asm__ volatile("fninit");

	klog("done\n");
}