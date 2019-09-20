#pragma once

#include <functional>
#include <memory>

namespace kernel{
    enum DescriptorTypes
    {
        STD_OUT = 1
    };
    
    class FileDescriptorHandler
    {
    public:
        typedef std::unique_ptr<FileDescriptorHandler> HandlerPtr;
        typedef std::function<void(uint8_t*, std::size_t)> WriteHandler;
        virtual WriteHandler get_write_handler() = 0;
    };
    
    class FileDescriptor
    {
    public:
        typedef int Id;
        FileDescriptor() = default;
        FileDescriptor(int id, FileDescriptorHandler::HandlerPtr&& handler)
            :m_handler(std::move(handler))
        {
            m_writeHandler = m_handler->get_write_handler();
        }
        
        void handle_write(void* ptr, std::size_t length)
        {
            m_writeHandler(reinterpret_cast<uint8_t*>(ptr), length);
        }

    private:
        FileDescriptorHandler::HandlerPtr m_handler;
        FileDescriptorHandler::WriteHandler m_writeHandler;
    };
}
