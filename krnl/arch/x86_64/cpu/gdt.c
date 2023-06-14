#include <arch.h>

#include <debug/log.h>

void gdt_init()
{
	gdt_entry_t gdt[5];
	gdtr_t gdtr;

	gdt_set_desc(&gdt[0], 0, 0, 0, 0);
	gdt_set_desc(&gdt[1], 0, 0xFFFFF, 0x9A, 0xA);
	gdt_set_desc(&gdt[2], 0, 0xFFFFF, 0x92, 0xC);
	gdt_set_desc(&gdt[3], 0, 0xFFFFF, 0xFA, 0xA);
	gdt_set_desc(&gdt[4], 0, 0xFFFFF, 0xF2, 0xC);
	//gdt_set_desc(&gdt[5], 0, 0, 0, 0); // TODO: TSS

	gdtr.size = sizeof(gdtr) - 1; // number of descriptors
	gdtr.offset = (uint64_t)&gdt[0];

	// Why is this looping infinitely???
	klog("Loading GDT...");

	gdt_load(&gdtr);
	gdt_reloadseg();
}

void gdt_set_desc(gdt_entry_t *gdt, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
	gdt->base_lo = (base & 0xFFFF);
	gdt->limit_lo = (limit & 0xFFFF);
	gdt->base_mid = (base & 0xFF0000) >> 16;
	gdt->limit_hi = (limit & 0xF0000) >> 16;
	gdt->base_hi = (base & 0xFF000000) >> 24;

	gdt->flags = (flags & 0xF);
	gdt->access = access;
}