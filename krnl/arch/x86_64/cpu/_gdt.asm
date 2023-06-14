;
; OSDev
; Copyright (C) 2023 Jozef Nagy
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;

gdtr dw 0
	 dq 0

global gdt_load
gdt_load:
	mov [gdtr], di
	mov [gdtr+2], rsi
	lgdt [gdtr]

global gdt_reloadseg
gdt_reloadseg:
	push 0x08
	lea rax, [rel .rel_cs]
	push rax
	retfq

.rel_cs:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret