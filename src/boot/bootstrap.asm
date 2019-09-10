.set MBALIGN,  1<<0
.set MEMINFO,  1<<1
.set MAGIC,    0x1BADB002
.set FLAGS,    MBALIGN | MEMINFO
.set CHECKSUM, -(MAGIC + FLAGS)
.set null_selector, 0x0
.set code_segment_selector, 0x8
.set data_segment_selector, 0x10

.section .multiboot
.align 4
.4byte MAGIC
.4byte FLAGS
.4byte CHECKSUM

.extern _main
.section .text
.code32
//GDT for flat memory model
.align 4
gdtr:
	.2byte gdt32mod_end - gdt32mod -1
	.4byte gdt32mod
	.4byte 0x0
gdt32mod:
	//Entry 0x0: Null desriptor
	.8byte 0x0
	//Entry 0x8: Code segment
	.2byte 0xffff		       //Limit
  	.2byte 0x0000		       //Base 15:00
	.byte  0x00			       //Base 23:16
	.byte  0x9a		           //Flags
	.byte  0xcf		           //Flags
	.byte  0x00			       //Base 32:24
	//Entry 0x10: Data segment
	.2byte 0xffff		       //Limit
	.2byte 0x0000		       //Base 15:00
	.byte  0x00			       //Base 23:16
	.byte  0x92		           //Flags
	.byte  0xcf		           //Flags
	.byte  0x00			       //Base 32:24
gdt32mod_end:
	.byte  0x00

.set stack_top, 0xa0000
.global _start
.type _start, @function
_start:
	lgdt (gdtr)
	ljmp $code_segment_selector, $_protected_mod
_protected_mod:
	// Set up 32-bit data segment
	mov $data_segment_selector, %ecx //move data segment selector
	// Set up stack
	mov %ecx, %ss
	mov $stack_top, %esp //set stack top, occupies about ~600kb
	// set up the current frame
	mov $stack_top, %ebp
	// Set up data segment
	mov %ecx, %ds
    // Nulyfing all the general selectors
	mov $null_selector, %ecx
	mov %ecx, %es
	mov %ecx, %fs
	mov %ecx, %gs
	//Fetch bootloader memory layout data
	push %eax //Push multiboot pointer
	push %ebx //Push magic value
	call _main 
	//if exit, disable interrupts and loop
.global _panic
.type _panic, @function
_panic:
	cli
b:	hlt
	jmp b
