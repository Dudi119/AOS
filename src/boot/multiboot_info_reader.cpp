#include "multiboot_info_reader.h"

extern "C" char _end;

namespace boot{
    MultiBootInfoReader::MultiBootInfoReader(multiboot_info& info, unsigned int magic)
        :m_info(info), m_is_high_memory_valid(false), m_is_memory_map_valid(false)
    {
        static_assert(sizeof(caddr_t) == sizeof(unsigned long), "mismatch between boot loader info memory value types and destination type");
        if(magic != bootloader_magic_value)
            return;
        if(m_info.flags & FlagProperty::MEMORY_VALID)
        {
            m_high_memory_high = reinterpret_cast<caddr_t>(m_info.mem_upper * memory_limit_granularity);
            m_high_memory_low = &_end;
            m_is_high_memory_valid = true;
        }
        
        m_is_memory_map_valid = m_info.flags & FlagProperty::MEMORY_MAP;
    }
}
