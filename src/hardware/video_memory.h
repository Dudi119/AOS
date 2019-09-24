#pragma once

#include <functional>
#include <utility>
#include <memory>
#include "memory.h"
#include "machine.h"
#include "kernel/file_descriptor.h"
#include "boot/multiboot_info_reader.h"

namespace hardware{
    template<typename SymbolPtr, std::size_t SymbolSize = sizeof(SymbolPtr)>
    class VideoMemory : public kernel::FileDescriptorHandler
    {
    public:
        typedef std::size_t Row;
        typedef std::size_t Column;
        typedef std::pair<Row, Column> Location;
        static const std::size_t SymbolSizeV = SymbolSize;
        VideoMemory(MemoryType type, Row row, Column column)
            :COLUMN_SIZE(column), ROW_SIZE(row)
        {
            const MemoryMappingEntry& entry = Machine::instance().get_memory().get_memory_region(type);
            m_start = reinterpret_cast<SymbolPtr*>(entry.Start);
            m_end = reinterpret_cast<SymbolPtr*>(entry.End);
            m_current = m_start;
        }
        virtual ~VideoMemory() = default;
        void write(uint8_t* str, std::size_t length)
        {
            static const uint8_t NewLine = '\n';
            Location currentLocation = get_current_location();
            Row row =  currentLocation.first;
            Column column = currentLocation.second;
        
            char* passEOL = strtok(reinterpret_cast<char*>(str), reinterpret_cast<const char*>(&NewLine));
            for(; row < ROW_SIZE;)
            {
                std::size_t symbolsToWrite = std::min(passEOL == nullptr ? length :
                                                      strlen(reinterpret_cast<const char*>(str)), COLUMN_SIZE - column);
                write_symbols(str, symbolsToWrite);
                length -= symbolsToWrite;
                if(!length)
                    return;
            
                if(symbolsToWrite == COLUMN_SIZE - column) //we filled a line with write
                {
                    drop_line(row);
                    str += symbolsToWrite;
                    column = 0;
                    row++;
                }
                else if(passEOL) //go down
                {
                    drop_line(row);
                    passEOL = strtok(nullptr, reinterpret_cast<const char*>(&NewLine));
                    str = reinterpret_cast<uint8_t*>(passEOL);
                    column = 0;
                    row++;
                    length--;
                }
                else //stay at the same line
                {
                    str += symbolsToWrite;
                    column = symbolsToWrite == (COLUMN_SIZE - column) ? 0 : column + symbolsToWrite;
                }
            }
        }
        WriteHandler get_write_handler()
        {
            return WriteHandler(std::bind(&VideoMemory::write, this, std::placeholders::_1, std::placeholders::_2));
        }
        
    private:
        virtual void write_symbols(uint8_t*& value, std::size_t symbolsToWrite) = 0;
        Location get_current_location()
        {
            std::size_t currentRow = (m_current - m_start) / (COLUMN_SIZE * SymbolSize);
            std::size_t currentColumn = ((m_current - m_start) / SymbolSize) % COLUMN_SIZE;
            return Location(currentRow, currentColumn);
        }
        void drop_line(Row row)
        {
            m_current = row < (ROW_SIZE - 1) ? m_start + (row + 1) * COLUMN_SIZE * SymbolSize :
                    m_start + (ROW_SIZE - 1) * COLUMN_SIZE * SymbolSize;
        }
        
    protected:
        const std::size_t COLUMN_SIZE;
        const std::size_t ROW_SIZE;
        SymbolPtr* m_start;
        SymbolPtr* m_end;
        SymbolPtr* m_current;
    };
    
    enum VGAColor : uint8_t
    {
        COLOR_WHITE = 15
    };
    
    class ColorTextVideoMemory : public VideoMemory<uint16_t>
    {
    public:
        ColorTextVideoMemory(Row row, Column column);
        ~ColorTextVideoMemory() override = default;
        
    private:
        void write_symbols(uint8_t*& value, std::size_t symbolsToWrite) override;
    };
    
    class TextVideoMemory : public VideoMemory<uint8_t>
    {
    public:
        TextVideoMemory(Row row, Column column);
        ~TextVideoMemory() override = default;
        
    private:
        void write_symbols(uint8_t*& value, std::size_t symbolsToWrite) override;
    };
    
    kernel::FileDescriptorHandler::HandlerPtr create_video_memory(const boot::VideoMemoryInfo& videoInfo);
}