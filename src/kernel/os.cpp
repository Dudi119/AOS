#include "os.h"
#include "boot/multiboot_info_reader.h"

extern "C" char _end;
extern "C" void(_init)(void);

namespace kernel{
    
    _reent OS::m_reent{};
    caddr_t OS::m_heapBegin = nullptr;
    caddr_t OS::m_heapEnd = nullptr;
    
    void OS::init(const boot::MultiBootInfoReader& reader)
    {
        init_c_runtime();
        init_heap(reader);
        _init();
    }
    
    void OS::init_c_runtime()
    {
        m_reent = _reent _REENT_INIT(m_reent);
        _REENT = &m_reent;
    }
    
    void OS::init_heap(const boot::MultiBootInfoReader& reader)
    {
        m_heapBegin = reader.get_high_memory_low();
        m_heapEnd = m_heapBegin;
    }
    
}
