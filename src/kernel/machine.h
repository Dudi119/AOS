#pragma once

namespace kernel {
    
    class Machine {
    public:
        static Machine &instance()
        {
            static Machine instance;
            return instance;
        }
    };
    
}

