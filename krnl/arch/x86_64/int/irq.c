#include <arch.h>

#include <debug/log.h>

irq_handler_t __handlers[16];

void irq_init(void)
{
	pic_init(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

	for (int i = 0; i < 16; i++) {
		isr_register(PIC_REMAP_OFFSET + i, irq_handler);
	}

	sti();
}

void irq_register(int irq, irq_handler_t handler)
{
	__handlers[irq] = handler;
	klog("Registered IRQ%i\n", irq);
}

void irq_deregister(int irq)
{
	__handlers[irq] = NULL;
	klog("Deregistered IRQ%i\n", irq);
}

void irq_handler(cpu_regs_t *regs)
{
	int irq = regs->int_no - PIC_REMAP_OFFSET;
	klog("Caught IRQ%i\n", irq);

	if (__handlers[irq] != NULL) {
		__handlers[irq](regs);
	} else {
		klog("Unhandled IRQ%i\n", irq);
	}

	pic_eoi(irq);
}