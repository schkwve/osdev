#ifndef __IRQ_H_
#define __IRQ_H_

typedef void (*irq_handler_t)(cpu_regs_t *regs);

void irq_init(void);

void irq_register(int irq, irq_handler_t handler);
void irq_deregister(int irq);

void irq_handler(cpu_regs_t *regs);

#endif // __IRQ_H_