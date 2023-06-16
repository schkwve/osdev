#ifndef __PIC_H_
#define __PIC_H_

#include <stdint.h>

#define PIC1_CMD	0x20
#define PIC1_DATA	0x21
#define PIC2_CMD	0xA0
#define PIC2_DATA	0xA1

#define PIC_ICW1_ICW4			0x01
#define PIC_ICW1_SINGLE			0x02
#define PIC_ICW1_INTERVAL		0x04
#define PIC_ICW1_LEVEL			0x08
#define PIC_ICW1_INIT			0x10

#define PIC_ICW4_8086			0x01
#define PIC_ICW4_AUTOEOI		0x02
#define PIC_ICW4_BUFFER_MASTER	0x04
#define PIC_ICW4_BUFFER_SLAVE	0x00
#define PIC_ICW4_BUFFERED		0x08
#define PIC_ICW4_SFNM			0x10

#define PIC_EOI					0x20
#define PIC_READ_IRR			0x0A
#define PIC_READ_ISR			0x0B

#define PIC_REMAP_OFFSET		0x20

void pic_init(uint8_t offset1, uint8_t offset2);

void pic_eoi(int irq);

uint16_t pic_get_irr();
uint16_t pic_get_isr();

void pic_mask(int irq);
void pic_unmask(int irq);

void pic_disable();

#endif // __PIC_H_