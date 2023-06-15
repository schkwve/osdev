[bits 64]

extern idtr

global idt_load
idt_load:
	lidt [idtr]
	ret