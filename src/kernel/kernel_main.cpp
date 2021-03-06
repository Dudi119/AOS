#include <stdio.h>
#include "boot/multiboot.h"
#include "boot/multiboot_info_reader.h"
#include "utils/logger.h"
#include "os.h"

extern "C" int _main(multiboot_info_t* mbi, unsigned int magic)
{
    boot::MultiBootInfoReader reader(*mbi, magic);
    kernel::OS::instance().init(reader);
    
	return 0;
}
