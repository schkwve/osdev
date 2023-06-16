#include <arch.h>

#include <stdint.h>

void pic_init(uint8_t offset1, uint8_t offset2)
{
	uint8_t mask1 = inb(PIC1_DATA);
	uint8_t mask2 = inb(PIC2_DATA);
	
	outb(PIC1_CMD, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
	io_wait();
	outb(PIC2_CMD, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
	io_wait();

	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();

	outb(PIC1_DATA, 0x4);
	io_wait();
	outb(PIC2_DATA, 0x2);
	io_wait();

	outb(PIC1_DATA, PIC_ICW4_8086);
	io_wait();
	outb(PIC2_DATA, PIC_ICW4_8086);
	io_wait();

	// restore masks
	outb(PIC1_DATA, mask1);
	io_wait();
	outb(PIC2_DATA, mask2);
	io_wait();
}

void pic_eoi(int irq)
{
	if (irq >= 8) {
		outb(PIC2_CMD, PIC_EOI);
	}

	outb(PIC1_CMD, PIC_EOI);
}

uint16_t pic_get_irr()
{
	outb(PIC1_CMD, PIC_READ_IRR);
	outb(PIC2_CMD, PIC_READ_IRR);

	return (inb(PIC2_CMD) | (inb(PIC2_CMD) << 8));
}

uint16_t pic_get_isr()
{
	outb(PIC1_CMD, PIC_READ_ISR);
	outb(PIC2_CMD, PIC_READ_ISR);

	return (inb(PIC2_CMD) | (inb(PIC2_CMD) << 8));
}

void pic_mask(int irq)
{
	uint8_t port;

	if (irq < 8) {
		port = PIC1_DATA;
	} else {
		irq -= 8;
		port = PIC2_DATA;
	}

	uint8_t mask = inb(port);
	outb(port, mask | (1 << irq));
}

void pic_unmask(int irq)
{
	uint8_t port;

	if (irq < 8) {
		port = PIC1_DATA;
	} else {
		irq -= 8;
		port = PIC2_DATA;
	}

	uint8_t mask = inb(port);
	outb(port, mask & ~(1 << irq));
}

void pic_disable()
{
	outb(PIC1_DATA, 0xFF);
	io_wait();
	outb(PIC2_DATA, 0xFF);
	io_wait();
}