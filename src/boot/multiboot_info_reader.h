#pragma once

#include <iterator>
#include <sys/types.h>
#include "multiboot.h"

namespace boot{
    enum FrameBufferType
    {
        INDEXED_COLOR = 0,
        RGB,
        EGA
    };
    
    class VideoMemoryInfo
    {
    public:
        static const std::size_t BPPS_8 = 8;
        static const std::size_t BPPS_16 = 16;
        static const std::size_t DEFAULT_WIDTH = 80;
        static const std::size_t DEFAULT_HEIGHT = 25;
        FrameBufferType Type;
        uintptr_t FrameBufferAddres;
        std::size_t Width;
        std::size_t Height;
        std::size_t Bpps;
    };
    
    class MultiBootInfoReader
    {
    public:
        class MemoryMap
        {
        public:
            class iterator :
                    public std::iterator<std::forward_iterator_tag,
                            multiboot_memory_map_t*, ptrdiff_t,
                            multiboot_memory_map_t*, multiboot_memory_map_t&>
            {
            public:
                iterator(multiboot_memory_map_t* ptr, std::size_t count)
                        :m_current(ptr), m_count(count)
                {}
                iterator& operator++()
                {
                    if(m_count)
                    {
                        m_current++;
                        m_count--;
                    }
                    return *this;
                }
            
                bool operator==(const iterator& other) const{return m_current == other.m_current;}
                bool operator!=(const iterator& other) const{return m_current != other.m_current;}
                reference operator*() {return *m_current;}
                pointer operator->() {return m_current;}
        
            private:
                multiboot_memory_map_t* m_current;
                std::size_t m_count;
            
            };
        
            MemoryMap() = default;
            MemoryMap(multiboot_memory_map_t* memoryMap, std::size_t count);
            iterator begin(){ return iterator(m_begin, m_count); }
            iterator end(){ return iterator(m_end, 0); }
    
        private:
            multiboot_memory_map_t* m_begin;
            multiboot_memory_map_t* m_end;
            std::size_t m_count;
        };
        
        MultiBootInfoReader(multiboot_info& info, unsigned int magic);
    
        bool is_high_memory_valid() const { return m_isHighMemoryValid;}
        bool is_memory_map_valid() const { return m_isMemoryMapValid;}
        uintptr_t get_high_memory_low() const {return m_highMemoryLow;};
        uintptr_t get_high_memory_high() const {return m_highMemoryHigh;};
        const VideoMemoryInfo& get_video_memory_info() const { return m_videoMemoryInfo; }
        MemoryMap& get_memory_map() { return m_mmap; }
    
    private:
        const unsigned long memory_limit_granularity = 1024;
        const unsigned int bootloader_magic_value = 0x2BADB002;
        enum FlagProperty : unsigned long
        {
            MEMORY_VALID = 0x1,
            MEMORY_MAP = 0x40,
            VIDEO_VALID = 0x1000
        };
        
    private:
        multiboot_info& m_info;
        bool m_isHighMemoryValid;
        bool m_isMemoryMapValid;
        uintptr_t m_highMemoryLow;
        uintptr_t m_highMemoryHigh;
        MemoryMap m_mmap;
        VideoMemoryInfo m_videoMemoryInfo;
    };
}