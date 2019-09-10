#include "machine.h"

extern "C" void(_panic)(void);

namespace hardware{
    
    void Machine::panic()
    {
        _panic();
    }
}
