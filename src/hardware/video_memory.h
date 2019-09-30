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
    protected:
        typedef std::unique_ptr<uint8_t, std::function<void(uint8_t*)>> BufferGuard;
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
            BufferGuard temporal_buf = BufferGuard(new uint8_t[length + 1], [](uint8_t* ptr){delete [] ptr;});
            std::memcpy(temporal_buf.get(), str, length);
            temporal_buf.get()[length] = '\0';
            uint8_t* temporalBufPtr = temporal_buf.get();
            
            Location currentLocation = get_current_location();
            Row row =  currentLocation.first;
            Column column = currentLocation.second;
        
            char* passEOL = strtok(reinterpret_cast<char*>(temporalBufPtr), reinterpret_cast<const char*>(&NewLine));
            while(length)
            {
                std::size_t symbolsToWrite = std::min(passEOL == nullptr ? length :
                                                      strlen(reinterpret_cast<const char*>(temporalBufPtr)), COLUMN_SIZE - column);
                if(row == ROW_SIZE - 1)
                    move_buffer_up();
    
                clear_single_line(row);
                write_symbols(temporalBufPtr, symbolsToWrite);
                length -= symbolsToWrite;
                if(!length)
                    return;
            
                if(symbolsToWrite == COLUMN_SIZE - column) //we filled a line with write
                {
                    drop_line(row);
                    temporalBufPtr += symbolsToWrite;
                    column = 0;
                    row = row == ROW_SIZE - 1 ? row : row + 1;
                }
                else if(passEOL) //go down
                {
                    drop_line(row);
                    passEOL = strtok(nullptr, reinterpret_cast<const char*>(&NewLine));
                    temporalBufPtr = reinterpret_cast<uint8_t*>(passEOL);
                    column = 0;
                    row = row == ROW_SIZE - 1 ? row : row + 1;
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
            for(Row row = 0; row < ROW_SIZE; row++)
            {
                clear_single_line(row);
            }
        }
        void clear_single_line(Row row)
        {
            uint8_t lineTemplate[COLUMN_SIZE + 1];
            for(Column column = 0; column < COLUMN_SIZE; column++)
            {
                lineTemplate[column] = ' ';
            }
            lineTemplate[COLUMN_SIZE] = '\0';
            
            Location prevLocation = get_current_location();
            set_current_location(row, 0);
            write_symbols(lineTemplate, COLUMN_SIZE);
            set_current_location(prevLocation.first, prevLocation.second);
        }
        WriteHandler get_write_handler()
        {
            return WriteHandler(std::bind(&VideoMemory::write, this, std::placeholders::_1, std::placeholders::_2));
        }
        Location set_current_location(Row row, Column column)
        {
            m_current = m_start + row * COLUMN_SIZE + column;
            return Location(row, column);
        }

    protected:
        SymbolType* get_row_buffer(Row row)
        {
            return m_start + row * COLUMN_SIZE;
        }

    private:
        void move_buffer_up()
        {
            for(Row row = 1; row < ROW_SIZE; row++)
                copy_one_row_up(row);
        }
        virtual void write_symbols(uint8_t* value, std::size_t symbolsToWrite) = 0;
        virtual BufferGuard get_row_value(Row row) = 0;
        void copy_one_row_up(Row row)
        {
           if(row == 0)
               return;
           if(row > ROW_SIZE)
               Machine::panic();
           
           Location prevLocation = get_current_location();
           clear_single_line(row - 1);
           BufferGuard rowBuffer = get_row_value(row);
           set_current_location(row - 1, 0);
           write_symbols(rowBuffer.get(), COLUMN_SIZE);
           set_current_location(prevLocation.first, prevLocation.second);
           
        }
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
        BufferGuard get_row_value(Row row) override;
    };
    
    class TextVideoMemory : public VideoMemory<uint8_t>
    {
    public:
        TextVideoMemory(Row row, Column column);
        ~TextVideoMemory() override = default;
        
    private:
        void write_symbols(uint8_t* value, std::size_t symbolsToWrite) override;
        BufferGuard get_row_value(Row row) override;
    };
    
    kernel::FileDescriptorHandler::HandlerPtr create_video_memory(const boot::VideoMemoryInfo& videoInfo);
}