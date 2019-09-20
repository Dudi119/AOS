#pragma once

#include "memory.h"

namespace kernel{
    class OS;
}

namespace hardware {
    
    class Machine {
    public:
        static Machine &instance()
        {
            static Machine instance;
            return instance;
        }
        
        const Memory& get_memory() const { return m_memory; }
        static void panic();
        
    private:
        Memory& get_mutable_memory() { return m_memory; }
        friend class kernel::OS;
        
    private:
        Memory m_memory;
    };
    
}

