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

#ifndef __VECTOR_H_
#define __VECTOR_H_

#include <utils/alloc.h>

#include <stdint.h>
#include <string.h>

#define VECTOR_RESIZE_FACTOR 4

#define vec_struct(type) \
	struct {             \
		size_t len;      \
		size_t capacity; \
		type *data;      \
	}

#define vec_extern(type, name) extern vec_struct(type) name
#define vec_new(type, name) vec_struct(type) name = { 0 }
#define vec_new_static(type, name) static vec_new(type, name)

#define vec_push_back(vec, elem)                                   \
	{                                                              \
		(vec)->len++;                                              \
		if ((vec)->capacity < (vec)->len * sizeof(elem)) {         \
			(vec)->capacity =                                      \
				(vec)->len * sizeof(elem) * VECTOR_RESIZE_FACTOR;  \
			(vec)->data = krealloc((vec)->data, (vec)->capacity); \
		}                                                          \
		(vec)->data[(vec)->len - 1] = elem;                        \
	}

#define vec_length(vec) (vec)->len
#define vec_at(vec, index) (vec)->data[index]

#define vec_erase(vec, index)                                    \
	{                                                            \
		memcpy(&((vec)->data[index]), &((vec)->data[index + 1]), \
			   sizeof((vec)->data[0]) * (vec)->len - index - 1); \
		(vec)->len--;                                            \
	}

#define vec_erase_val(vec, val)                         \
	{                                                   \
		for (size_t __i = 0; __i < (vec)->len; __i++) { \
			if (vec_at(vec, __i) == (val)) {            \
				vec_erase(vec, __i);                    \
				break;                                  \
			}                                           \
		}                                               \
	}

#endif // __VECTOR_H_