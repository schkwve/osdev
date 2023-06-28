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

#ifndef __CPU_H_
#define __CPU_H_

#include <cpu/gdt.h>
#include <utils/utils.h>

#include <stdint.h>
#include <stdbool.h>

#define ECX_SSE3 (1 << 0) // Streaming SIMD Extensions 3
#define ECX_PCLMULQDQ (1 << 1) // PCLMULQDQ Instruction
#define ECX_DTES64 (1 << 2) // 64-Bit Debug Store Area
#define ECX_MONITOR (1 << 3) // MONITOR/MWAIT
#define ECX_DS_CPL (1 << 4) // CPL Qualified Debug Store
#define ECX_VMX (1 << 5) // Virtual Machine Extensions
#define ECX_SMX (1 << 6) // Safer Mode Extensions
#define ECX_EST (1 << 7) // Enhanced SpeedStep Technology
#define ECX_TM2 (1 << 8) // Thermal Monitor 2
#define ECX_SSSE3 (1 << 9) // Supplemental Streaming SIMD Extensions 3
#define ECX_CNXT_ID (1 << 10) // L1 Context ID
#define ECX_FMA (1 << 12) // Fused Multiply Add
#define ECX_CX16 (1 << 13) // CMPXCHG16B Instruction
#define ECX_XTPR (1 << 14) // xTPR Update Control
#define ECX_PDCM (1 << 15) // Perf/Debug Capability MSR
#define ECX_PCID (1 << 17) // Process-context Identifiers
#define ECX_DCA (1 << 18) // Direct Cache Access
#define ECX_SSE41 (1 << 19) // Streaming SIMD Extensions 4.1
#define ECX_SSE42 (1 << 20) // Streaming SIMD Extensions 4.2
#define ECX_X2APIC (1 << 21) // Extended xAPIC Support
#define ECX_MOVBE (1 << 22) // MOVBE Instruction
#define ECX_POPCNT (1 << 23) // POPCNT Instruction
#define ECX_TSC (1 << 24) // Local APIC supports TSC Deadline
#define ECX_AESNI (1 << 25) // AESNI Instruction
#define ECX_XSAVE (1 << 26) // XSAVE/XSTOR States
#define ECX_OSXSAVE (1 << 27) // OS Enabled Extended State Management
#define ECX_AVX (1 << 28) // AVX Instructions
#define ECX_F16C (1 << 29) // 16-bit Floating Point Instructions
#define ECX_RDRAND (1 << 30) // RDRAND Instruction

#define EDX_FPU (1 << 0) // Floating-Point Unit On-Chip
#define EDX_VME (1 << 1) // Virtual 8086 Mode Extensions
#define EDX_DE (1 << 2) // Debugging Extensions
#define EDX_PSE (1 << 3) // Page Size Extension
#define EDX_TSC (1 << 4) // Time Stamp Counter
#define EDX_MSR (1 << 5) // Model Specific Registers
#define EDX_PAE (1 << 6) // Physical Address Extension
#define EDX_MCE (1 << 7) // Machine-Check Exception
#define EDX_CX8 (1 << 8) // CMPXCHG8 Instruction
#define EDX_APIC (1 << 9) // APIC On-Chip
#define EDX_SEP (1 << 11) // SYSENTER/SYSEXIT instructions
#define EDX_MTRR (1 << 12) // Memory Type Range Registers
#define EDX_PGE (1 << 13) // Page Global Bit
#define EDX_MCA (1 << 14) // Machine-Check Architecture
#define EDX_CMOV (1 << 15) // Conditional Move Instruction
#define EDX_PAT (1 << 16) // Page Attribute Table
#define EDX_PSE36 (1 << 17) // 36-bit Page Size Extension
#define EDX_PSN (1 << 18) // Processor Serial Number
#define EDX_CLFLUSH (1 << 19) // CLFLUSH Instruction
#define EDX_DS (1 << 21) // Debug Store
#define EDX_ACPI (1 << 22) // Thermal Monitor and Software Clock Facilities
#define EDX_MMX (1 << 23) // MMX Technology
#define EDX_FXSR (1 << 24) // FXSAVE and FXSTOR Instructions
#define EDX_SSE (1 << 25) // Streaming SIMD Extensions
#define EDX_SSE2 (1 << 26) // Streaming SIMD Extensions 2
#define EDX_SS (1 << 27) // Self Snoop
#define EDX_HTT (1 << 28) // Multi-Threading
#define EDX_TM (1 << 29) // Thermal Monitor
#define EDX_PBE (1 << 31) // Pending Break Enable

#define EDX_SYSCALL (1 << 11) // SYSCALL/SYSRET
#define EDX_XD (1 << 20) // Execute Disable Bit
#define EDX_1GB_PAGE (1 << 26) // 1 GB Pages
#define EDX_RDTSCP (1 << 27) // RDTSCP and IA32_TSC_AUX
#define EDX_64_BIT (1 << 29) // 64-bit Architecture

#define MSR_GS_BASE 0xC0000101
#define MSR_GS_KERNEL_BASE 0xC0000102

typedef struct {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rbp;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	uint64_t int_no;
	uint64_t err;

	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} cpu_regs_t;

typedef enum {
	PAT_UNCACHEABLE = 0,
	PAT_WRITE_COMBINING = 1,
	PAT_WRITE_THROUGH = 4,
	PAT_WRITE_PROTECTED = 5,
	PAT_WRITE_BACK = 6,
	PAT_UNCACHED = 7
} pat_cache_t;

void cpu_check(void);

extern void write_msr(uint32_t msr, uint64_t val);
extern uint64_t read_msr(uint32_t msr);

#endif // __CPU_H_