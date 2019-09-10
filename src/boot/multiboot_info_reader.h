#pragma once

#include <iterator>
#include <sys/types.h>
#include "multiboot.h"

namespace boot{
    class MultiBootInfoReader
    {
    public:
        MultiBootInfoReader(multiboot_info& info, unsigned int magic);
    
        bool is_high_memory_valid() const { return m_is_high_memory_valid;}
        bool is_memory_map_valid() const { return m_is_memory_map_valid;}
        uintptr_t get_high_memory_low() const {return m_high_memory_low;};
        uintptr_t get_high_memory_high() const {return m_high_memory_high;};
    
    private:
        const unsigned long memory_limit_granularity = 1024;
        const unsigned int bootloader_magic_value = 0x2BADB002;
        enum FlagProperty : unsigned long
        {
            MEMORY_VALID = 0x1,
            MEMORY_MAP = 0x40
        };
        /*
        class MemoryMap
        {
        public:
            class iterator : public std::iterator<std::forward_iterator_tag, memory_map_t*, ptrdiff_t, memory_map_t*, memory_map_t&>
            {
            public:
                iterator(memory_map_t* ptr, std::size_t count)
                        :m_current(ptr ? ptr : m_sentinal), m_count(count)
                {}
                iterator& operator++()
                {
                    if(m_count)
                    {
                        m_current++;
                        m_count--;
                    }
                    else
                        m_current = m_sentinal;
                    
                    return *this;
                }
                bool operator==(const iterator& other) const{return m_current == other.m_current;}
                bool operator!=(const iterator& other) const{return m_current != other.m_current;}
                reference operator*() const{return *m_current;}
                pointer operator->() const{return m_current;}
            
            private:
                memory_map_t* m_current;
                std::size_t m_count;
                static memory_map_t* m_sentinal;
                
            };
        
        private:
            MemoryMap(memory_map_t& memoryMap, std::size_t count);
            iterator begin(){return iterator()}
            
        };
    */
    private:
        multiboot_info& m_info;
        bool m_is_high_memory_valid;
        bool m_is_memory_map_valid;
        uintptr_t m_high_memory_low;
        uintptr_t m_high_memory_high;
    };
}