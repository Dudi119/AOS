#pragma once
#ifndef _LIBCPP_NO_EXCEPTIONS
#define _LIBCPP_NO_EXCEPTIONS
#endif
#include <vector>

namespace hardware{
    
    enum MemoryType : char
    {
        VIDEO_MEMORY_MONO_TEXT,
        VIDEO_MEMORY_COLOR_TEXT
    };
    
    struct MemoryMappingEntry
    {
    public:
        MemoryType Type;
        uintptr_t Start;
        uintptr_t End;
    };
    
    class Memory
    {
    public:
        typedef std::vector<MemoryMappingEntry> MemoryMapping;
        void add_memory_region(MemoryType type, uintptr_t start, uintptr_t end);
        const MemoryMappingEntry& get_memory_region(MemoryType type);
        
    private:
        MemoryMapping m_mapping;
    };
}

