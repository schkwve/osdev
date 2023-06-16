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

#include <arch.h>
#include <debug/log.h>

#include <stdint.h>

extern int vectors[256];
extern uint64_t isr_tbl[];

isr_handler_t isr_handlers[256];

const char *isr_exceptions[] = { "#DE: Division Error",
								 "#DB: Debug Exception",
								 " — : NMI Interrupt",
								 "#BP: Breakpoint",
								 "#OF: Overflow",
								 "#BR: Bound Range Exceeded",
								 "#UD: Invalid Opcode",
								 "#NM: Device Not Available",
								 "#DF: Double Fault",
								 " — : Coprocessor Segment Overrun",
								 "#TS: Invalid TSS",
								 "#NP: Segment Not Present",
								 "#SS: Stack-Segment Fault",
								 "#GP: General Protection",
								 "#PF: Page Fault",
								 " — : Reserved",
								 "#MF: x87 FPU Floating-Point Error",
								 "#AC: Alignment Check",
								 "#MC: Machine Check",
								 "#XM: SIMD Floating-Point Exception",
								 "#VE: Virtualization Exception",
								 "#CP: Control Protection Exception",
								 " — : Reserved",
								 " — : Reserved",
								 " — : Reserved",
								 " — : Reserved",
								 " — : Reserved",
								 " — : Reserved",
								 "#HV: Hypervisor Injection Exception",
								 "#VC: VMM Communication Exception",
								 "#SX: Security Exception",
								 " — : Reserved" };

void isr_handler(cpu_regs_t *regs)
{
	if (isr_handlers[regs->int_no] != NULL)
		if (regs->int_no < 32) {
			klog(" EXCEPTION OCCURED: %s (%i)\n\n",
				 isr_exceptions[regs->int_no], regs->int_no);
			klog(
				"  RAX: 0x%.16llx, RBX:    0x%.16llx, RCX: 0x%.16llx, RDX: 0x%.16llx\n",
				regs->rax, regs->rbx, regs->rcx, regs->rdx);
			klog(
				"  RSI: 0x%.16llx, RDI:    0x%.16llx, RBP: 0x%.16llx, R8 : 0x%.16llx\n",
				regs->rsi, regs->rdi, regs->rbp, regs->r8);
			klog(
				"  R9 : 0x%.16llx, R10:    0x%.16llx, R11: 0x%.16llx, R12: 0x%.16llx\n",
				regs->r9, regs->r10, regs->r11, regs->r12);
			klog(
				"  R13: 0x%.16llx, R14:    0x%.16llx, R15: 0x%.16llx, SS : 0x%.16llx\n",
				regs->r13, regs->r14, regs->r15, regs->ss);
			klog(
				"  RSP: 0x%.16llx, RFLAGS: 0x%.16llx, CS : 0x%.16llx, RIP: 0x%.16llx\n",
				regs->rsp, regs->rflags, regs->cs, regs->rip);

			// TODO: implement panic()
			for (;;) {
				__asm__ volatile("hlt");
			}
		}
}

void isr_register(uint8_t vector, isr_handler_t handler)
{
	idt_set_desc(vector, isr_tbl[vector], 0x80, 0);
	isr_handlers[vector] = handler;
	klog("Registed INT#%i\n", vector);
}

void isr_deregister(uint8_t vector)
{
	idt_set_desc(vector, 0, 0, 0);
	isr_handlers[vector] = NULL;
	klog("Deregisted INT#%i\n", vector);
}