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
        static void init(const boot::MultiBootInfoReader& reader);
        
    private:
        static void init_c_runtime();
        static void init_heap(const boot::MultiBootInfoReader& reader);
        
    private:
        static _reent m_reent;
        static caddr_t m_heapBegin;
        static caddr_t m_heapEnd;
    };
}