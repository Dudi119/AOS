#include "memory.h"
#include "machine.h"

namespace hardware{

    void Memory::add_memory_region(hardware::MemoryType type, uintptr_t start, uintptr_t end)
    {
        auto it = std::find_if(m_mapping.begin(), m_mapping.end(), [&type](MemoryMappingEntry& entry){return entry.Type == type;});
        if(it != m_mapping.end())
        {
            Machine::panic();
        }
        m_mapping.emplace_back(MemoryMappingEntry{type, start, end});
    }
    
    const MemoryMappingEntry& Memory::get_memory_region(hardware::MemoryType type)
    {
        auto it = std::find_if(m_mapping.begin(), m_mapping.end(), [&type](MemoryMappingEntry& entry){return entry.Type == type;});
        if(it == m_mapping.end())
        {
            //throw exception once supported
        }
        return *it;
    }
}
