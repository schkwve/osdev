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

[bits 64]

extern isr_handler

%include "krnl/arch/x86_64/asm_utils.inc"

_isr_handler:
	cld
	pushaq

	mov rdi, rsp
	call isr_handler
	mov rsp, rax
	
	popaq
	add rsp, 16
	iretq

%macro isr_err 1
isr_%+%1:
	push %1
	jmp _isr_handler
%endmacro

%macro isr_noerr 1
isr_%+%1:
	push 0
	push %1
	jmp _isr_handler
%endmacro

isr_noerr 0
isr_noerr 1
isr_noerr 2
isr_noerr 3
isr_noerr 4
isr_noerr 5
isr_noerr 6
isr_noerr 7
isr_err 8
isr_noerr 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
isr_noerr 15
isr_noerr 16
isr_err 17
isr_noerr 18
isr_noerr 19
isr_noerr 20
isr_noerr 21
isr_noerr 22
isr_noerr 23
isr_noerr 24
isr_noerr 25
isr_noerr 26
isr_noerr 27
isr_noerr 28
isr_noerr 29
isr_err 30
isr_noerr 31

global isr_tbl
isr_tbl:
%assign i 0
%rep 32
	dq isr_%+i
%assign i i+1
%endrep