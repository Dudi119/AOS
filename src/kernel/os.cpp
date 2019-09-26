#include "os.h"
#include <stdio.h>
#include "boot/multiboot_info_reader.h"
#include "hardware/machine.h"
#include "hardware/memory.h"
#include "hardware/video_memory.h"
#include "file_descriptor.h"

extern "C" char _end;
extern "C" void(_init)(void);

namespace hw = hardware;

namespace kernel{
    
    void OS::init(const boot::MultiBootInfoReader& reader)
    {
        init_c_runtime();
        init_heap(reader);
        _init();
        
        hw::Memory& memory = hw::Machine::instance().get_mutable_memory();
        memory.add_memory_region(hw::VIDEO_MEMORY_MONO_TEXT, 0xB0000, 0xB7FFF);
        memory.add_memory_region(hw::VIDEO_MEMORY_COLOR_TEXT, 0xB8000, 0xBFFFF);
    
        init_file_descriptors(reader);
    }
    
    void OS::init_c_runtime()
    {
        m_reent = (struct _reent) _REENT_INIT(m_reent);
        _REENT = &m_reent;
    }
    
    void OS::init_heap(const boot::MultiBootInfoReader& reader)
    {
        m_heapBegin = reader.get_high_memory_low();
        m_heapEnd = m_heapBegin;
        m_heapLimit = reader.get_high_memory_high();
    }
    
    void OS::init_file_descriptors(const boot::MultiBootInfoReader& reader)
    {
        FileDescriptorHandler::HandlerPtr handlerPtr =
                hw::create_video_memory(reader.get_video_memory_info());
        m_descriptors.insert({DescriptorTypes::STD_OUT,
                              FileDescriptor(DescriptorTypes::STD_OUT, std::move(handlerPtr))});
    }
    
    FileDescriptor& OS::get_file_descriptor(FileDescriptor::Id id)
    {
        Descriptors::iterator it = m_descriptors.find(id);
        if(it == m_descriptors.end())
        {
            //throw exception once supported
            hw::Machine::panic();
        }
        return it->second;
    }
    
    void OS::print_logo()
    {
        printf("                         ....###.....#######...######.                         \n");
        printf("                         ...##.##...##.....##.##....##                         \n");
        printf("                         ..##...##..##.....##.##......                         \n");
        printf("                         .##.....##.##.....##..######.                         \n");
        printf("                         .#########.##.....##.......##                         \n");
        printf("                         .##.....##.##.....##.##....##                         \n");
        printf("                         .##.....##..#######...######.                         \n");
    }
}
