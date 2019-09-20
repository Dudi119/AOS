#include "video_memory.h"
#include "machine.h"

namespace hardware{
    ColorTextVideoMemory::ColorTextVideoMemory()
            :VideoMemory(MemoryType::VIDEO_MEMORY_COLOR_TEXT)
    {
    }
    
    void ColorTextVideoMemory::write_symbols(uint8_t*& value, std::size_t symbolsToWrite)
    {
    }
    
    TextVideoMemory::TextVideoMemory()
        :VideoMemory(MemoryType::VIDEO_MEMORY_MONO_TEXT)
    {
    }
    
    
    void TextVideoMemory::write_symbols(uint8_t*& value, std::size_t symbolsToWrite)
    {
        std::memcpy(m_current, value, symbolsToWrite);
        m_current += symbolsToWrite;
    }
    
    kernel::FileDescriptorHandler::HandlerPtr create_video_memory(MemoryType type)
    {
        typedef kernel::FileDescriptorHandler::HandlerPtr HandlerPtr;
        switch(type)
        {
            case MemoryType::VIDEO_MEMORY_MONO_TEXT:
            {
                return HandlerPtr(new TextVideoMemory());
            }
            case MemoryType::VIDEO_MEMORY_COLOR_TEXT:
            {
                return HandlerPtr(new ColorTextVideoMemory());
            }
            default:
            {
                Machine::panic();
                return nullptr;
            }
        }
    }
}
