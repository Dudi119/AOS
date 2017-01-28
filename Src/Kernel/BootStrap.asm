MBALIGN equ 1<<0
MEMINFO equ 1<<1
MAGIC equ 0x1BADB002
FLAGS equ MBALIGN | MEMINFO
CHECKSUM equ -(MAGIC + FLAGS)
; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.
section .multiboot
align 4
dd MAGIC
dd FLAGS  
dd CHECKSUM
; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
extern _main
section .text
;GDT for flat memory model
USE32
align 4
gdtr:
	dw gdt32mod_end -gdt32mod -1
	dd gdt32mod
gdt32mod:
	;; Entry 0x0: Null desriptor
	dq 0x0
	;; Entry 0x8: Code segment
	dw 0xffff		       ;Limit
  	dw 0x0000		       ;Base 15:00
	db 0x00			       ;Base 23:16
	dw 0xcf9a		       ;Flags
	db 0x00			       ;Base 32:24
	;; Entry 0x10: Data segment
	dw 0xffff		       ;Limit
	dw 0x0000		       ;Base 15:00
	db 0x00			       ;Base 23:16
	dw 0xcf92		       ;Flags
	db 0x00			       ;Base 32:24
gdt32mod_end:
	db 0x00

%define stack_top 0xA0000
global _start:function
_start:
	mov eax, 1
	lgdt [gdtr] ;load GDT
	;; Set up 32-bit data segment
	mov eax,0x10 ;move data segment selector
	;; Set up stack
	mov ss,eax
	mov esp,stack_top ;set stack top, occupies about ~600kb
	;; set up the current frame
	mov ebp,stack_top
	;; Set up data segment
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	;Fetch bootloader memory layout data
	push dword eax ;Push multiboot pointer
	push dword ebx ;Push magic value
	call _main 
	;if exit, disable interrupts and loop
	cli
b:	hlt
	jmp b
