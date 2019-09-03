/* Bootloader will look at this image and start execution at the symbol deignated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 1M;

	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text : ALIGN(0x20)
	{
		*(.multiboot)
		*(.text)
	}

	/*global costs- provilage Read-only data. */
	.rodata : ALIGN(0x20)
	{
		*(.rodata)
	}

	/*static storage Read-write data (initialized) */
	.data : ALIGN(0x20)
	{
		*(.data)
	}

	/*static storage Read-write data (uninitialized)*/
	.bss : ALIGN(0x20)
	{
		*(COMMON)
		*(.bss)
	}

	_end = .;
}
