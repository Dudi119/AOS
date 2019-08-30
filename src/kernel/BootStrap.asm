.set MBALIGN,  1<<0
.set MEMINFO,  1<<1
.set MAGIC,    0x1BADB002
.set FLAGS,    MBALIGN | MEMINFO
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.4byte MAGIC
.4byte FLAGS
.4byte CHECKSUM
.extern _main
.section .text
//GDT for flat memory model
.align 4
gdtr:
	.2byte gdt32mod_end -gdt32mod -1
	.4byte gdt32mod
gdt32mod:
	//Entry 0x0: Null desriptor
	.8byte 0x0
	//Entry 0x8: Code segment
	.2byte 0xffff		       //Limit
  	.2byte 0x0000		       //Base 15:00
	.byte  0x00			       //Base 23:16
	.2byte 0xcf9a		       //Flags
	.byte  0x00			       //Base 32:24
	//Entry 0x10: Data segment
	.2byte 0xffff		       //Limit
	.2byte 0x0000		       //Base 15:00
	.byte  0x00			       //Base 23:16
	.2byte 0xcf92		       //Flags
	.byte  0x00			       //Base 32:24
gdt32mod_end:
	.byte  0x00

.set stack_top, 0xA0000
.global _start
.type _start, @function
_start:
	mov $0x1, %eax
	lgdt (gdtr)
	// Set up 32-bit data segment
	mov $0x10, %eax //move data segment selector
	// Set up stack
	mov %eax, %ss
	mov $stack_top, %esp //set stack top, occupies about ~600kb
	// set up the current frame
	mov $stack_top, %ebp
	// Set up data segment
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	//Fetch bootloader memory layout data
	push %eax //Push multiboot pointer
	push %ebx //Push magic value
	call _main 
	//if exit, disable interrupts and loop
	cli
b:	hlt
	jmp b
