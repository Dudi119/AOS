#pragma once

#include <sys/reent.h>
#include <sys/types.h>

namespace boot{
    class MultiBootInfoReader;
}

namespace kernel{
    class OS
    {
    public:
        static OS& instance()
        {
            static OS instance;
            return instance;
        }
        void init(const boot::MultiBootInfoReader& reader);
        uintptr_t& get_heap_begin() {return m_heapBegin;}
        uintptr_t& get_heap_end() {return m_heapEnd;}
        uintptr_t get_heap_limit() const {return m_heapLimit;}
        
    private:
        OS() = default;
        void init_c_runtime();
        void init_heap(const boot::MultiBootInfoReader& reader);
        
    private:
        _reent m_reent;
        uintptr_t m_heapBegin;
        uintptr_t m_heapEnd;
        uintptr_t m_heapLimit;
    };
}