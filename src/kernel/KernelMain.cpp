#include "../multiboot/Multiboot.h"
#include <stdio.h>

extern "C" int _main(multiboot_info_t* mbd, unsigned int magic)
{
	printf("Hello world");
	return 0;
}
