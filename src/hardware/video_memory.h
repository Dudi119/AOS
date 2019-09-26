#pragma once

#include <functional>
#include <utility>
#include <memory>
#include "memory.h"
#include "machine.h"
#include "kernel/file_descriptor.h"
#include "boot/multiboot_info_reader.h"

namespace hardware{
    template<typename SymbolType, std::size_t SymbolSize = sizeof(SymbolType)>
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
            m_start = reinterpret_cast<SymbolType*>(entry.Start);
            m_end = reinterpret_cast<SymbolType*>(entry.End);
            m_current = m_start;
        }
        virtual ~VideoMemory() = default;
        void write(uint8_t* str, std::size_t length)
        {
            static const uint8_t NewLine = '\n';
            typedef std::unique_ptr<uint8_t, std::function<void(uint8_t*)>> BufferGuard;
            BufferGuard temporal_buf = BufferGuard(new uint8_t[length + 1], [](uint8_t* ptr){delete [] ptr;});
            std::memcpy(temporal_buf.get(), str, length);
            temporal_buf.get()[length] = '\0';
            uint8_t* temporalBufPtr = temporal_buf.get();
            
            Location currentLocation = get_current_location();
            Row row =  currentLocation.first;
            Column column = currentLocation.second;
        
            char* passEOL = strtok(reinterpret_cast<char*>(temporalBufPtr), reinterpret_cast<const char*>(&NewLine));
            for(; row < ROW_SIZE;)
            {
                std::size_t symbolsToWrite = std::min(passEOL == nullptr ? length :
                                                      strlen(reinterpret_cast<const char*>(temporalBufPtr)), COLUMN_SIZE - column);
                write_symbols(temporalBufPtr, symbolsToWrite);
                length -= symbolsToWrite;
                if(!length)
                    return;
            
                if(symbolsToWrite == COLUMN_SIZE - column) //we filled a line with write
                {
                    drop_line(row);
                    temporalBufPtr += symbolsToWrite;
                    column = 0;
                    row++;
                }
                else if(passEOL) //go down
                {
                    drop_line(row);
                    passEOL = strtok(nullptr, reinterpret_cast<const char*>(&NewLine));
                    temporalBufPtr = reinterpret_cast<uint8_t*>(passEOL);
                    column = 0;
                    row++;
                    length--;
                }
                else //stay at the same line
                {
                    temporalBufPtr += symbolsToWrite;
                    column = symbolsToWrite == (COLUMN_SIZE - column) ? 0 : column + symbolsToWrite;
                }
            }
        }
        void clear_screen()
        {
            uint8_t lineTemplate[COLUMN_SIZE + 1];
            for(Column column = 0; column < COLUMN_SIZE; column++)
            {
                lineTemplate[column] = ' ';
            }
            lineTemplate[COLUMN_SIZE] = '\0';
            for(Row row = 0; row < ROW_SIZE; row++)
            {
                write_symbols(lineTemplate, COLUMN_SIZE);
            }
        }
        WriteHandler get_write_handler()
        {
            return WriteHandler(std::bind(&VideoMemory::write, this, std::placeholders::_1, std::placeholders::_2));
        }
        
    private:
        virtual void write_symbols(uint8_t* value, std::size_t symbolsToWrite) = 0;
        Location get_current_location()
        {
            std::size_t bytesCount = (m_current - m_start) * SymbolSizeV;
            std::size_t currentRow = bytesCount / (COLUMN_SIZE * SymbolSize);
            std::size_t currentColumn = (bytesCount / SymbolSize) % COLUMN_SIZE;
            return Location(currentRow, currentColumn);
        }
        void drop_line(Row row)
        {
            m_current = row < (ROW_SIZE - 1) ? m_start + (row + 1) * COLUMN_SIZE:
                    m_start + (ROW_SIZE - 1) * COLUMN_SIZE;
        }
        
    protected:
        const std::size_t COLUMN_SIZE;
        const std::size_t ROW_SIZE;
        SymbolType* m_start;
        SymbolType* m_end;
        SymbolType* m_current;
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
        void write_symbols(uint8_t* value, std::size_t symbolsToWrite) override;
    };
    
    class TextVideoMemory : public VideoMemory<uint8_t>
    {
    public:
        TextVideoMemory(Row row, Column column);
        ~TextVideoMemory() override = default;
        
    private:
        void write_symbols(uint8_t* value, std::size_t symbolsToWrite) override;
    };
    
    kernel::FileDescriptorHandler::HandlerPtr create_video_memory(const boot::VideoMemoryInfo& videoInfo);
}