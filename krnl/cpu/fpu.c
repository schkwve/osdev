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