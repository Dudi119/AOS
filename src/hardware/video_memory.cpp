#include "video_memory.h"
#include "machine.h"

namespace hardware{
    ColorTextVideoMemory::ColorTextVideoMemory(Row row, Column column)
            :VideoMemory(MemoryType::VIDEO_MEMORY_COLOR_TEXT, row, column)
    {
        clear_screen();
        m_current = m_start;
    }
    
    void ColorTextVideoMemory::write_symbols(uint8_t* value, std::size_t symbolsToWrite)
    {
        typedef std::unique_ptr<uint16_t, std::function<void(uint16_t*)>> BufferGuard;
        BufferGuard temporal_buf = BufferGuard(new uint16_t[symbolsToWrite], [](uint16_t* ptr){delete [] ptr;});
        for(std::size_t idx = 0; idx < symbolsToWrite; idx++)
        {
            uint16_t symbol = VGAColor::COLOR_WHITE;
            temporal_buf.get()[idx] = symbol << 8 | *(value + idx);
        }
        std::memcpy(m_current, reinterpret_cast<char*>(temporal_buf.get()), symbolsToWrite * SymbolSizeV);
        m_current += symbolsToWrite;
    }
    
    TextVideoMemory::TextVideoMemory(Row row, Column column)
        :VideoMemory(MemoryType::VIDEO_MEMORY_MONO_TEXT, row, column)
    {
        clear_screen();
        m_current = m_start;
    }
    
    
    void TextVideoMemory::write_symbols(uint8_t* value, std::size_t symbolsToWrite)
    {
        std::memcpy(m_current, value, symbolsToWrite);
        m_current += symbolsToWrite;
    }
    
    kernel::FileDescriptorHandler::HandlerPtr create_video_memory(const boot::VideoMemoryInfo& videoInfo)
    {
        typedef kernel::FileDescriptorHandler::HandlerPtr HandlerPtr;
        switch(videoInfo.Bpps)
        {
            case boot::VideoMemoryInfo::BPPS_8:
            {
                return HandlerPtr(new TextVideoMemory(videoInfo.Height, videoInfo.Width));
            }
            case boot::VideoMemoryInfo::BPPS_16:
            {
                return HandlerPtr(new ColorTextVideoMemory(videoInfo.Height, videoInfo.Width));
            }
            default:
            {
                Machine::panic();
                return nullptr;
            }
        }
    }
}
