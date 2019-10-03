#include "multiboot_info_reader.h"

extern "C" char _end;

namespace boot{
    
    MultiBootInfoReader::MultiBootInfoReader(multiboot_info& info, unsigned int magic)
        :m_info(info), m_isHighMemoryValid(false), m_isMemoryMapValid(false)
    {
        static_assert(sizeof(caddr_t) == sizeof(uintptr_t), "mismatch between boot loader info memory value types and destination type");
        if(magic != bootloader_magic_value)
            return;
        if(m_info.flags & FlagProperty::MEMORY_VALID)
        {
            m_highMemoryHigh = 0x100000 + m_info.mem_upper * memory_limit_granularity - 1;
            m_highMemoryLow = reinterpret_cast<uintptr_t>(&_end);
            m_isHighMemoryValid = true;
        }
        
        m_isMemoryMapValid = m_info.flags & FlagProperty::MEMORY_MAP;
        if(m_isMemoryMapValid)
        {
            m_mmap = MemoryMap(reinterpret_cast<multiboot_memory_map_t*>(m_info.mmap_addr), m_info.mmap_length / sizeof(multiboot_memory_map_t));
        }
        
        if(m_info.flags & FlagProperty::VIDEO_VALID)
        {
            m_videoMemoryInfo.Type = (FrameBufferType)m_info.framebuffer_type;
            m_videoMemoryInfo.FrameBufferAddres = m_info.framebuffer_addr;
            m_videoMemoryInfo.Width = m_info.framebuffer_width;
            m_videoMemoryInfo.Height = m_info.framebuffer_height;
            m_videoMemoryInfo.Bpps = m_info.framebuffer_bpp;
        }
        else
        {
            m_videoMemoryInfo.Type = FrameBufferType::EGA;
            m_videoMemoryInfo.Width = VideoMemoryInfo::DEFAULT_WIDTH;
            m_videoMemoryInfo.Height = VideoMemoryInfo::DEFAULT_HEIGHT;
            m_videoMemoryInfo.Bpps = VideoMemoryInfo::BPPS_16;
        }
    }
    
    MultiBootInfoReader::MemoryMap::MemoryMap(multiboot_memory_map_t* memoryMap, std::size_t count)
        :m_begin(memoryMap), m_end(memoryMap + count), m_count(count)
    {
    }
}
