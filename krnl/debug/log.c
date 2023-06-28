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

#include <debug/log.h>
#include <dd/serial/serial.h>

#include "printf.h"

char klog_buf[4096];

void _klog(char *fmt, ...)
{
	va_list ptr;
	va_start(ptr, fmt);

	vsnprintf((char *)&klog_buf, -1, fmt, ptr);
	serial_write(klog_buf);

	va_end(ptr);
}