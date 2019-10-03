#pragma once

#include <string>
#include <vector>
#include <cstdarg>
#include "source.h"

namespace utils{
    enum TraceSeverity : short
    {
        Verbose = 1,
        Info,
        Fatal = 4,
        NoneWorking = 5
    };
    
    class Logger
    {
    public:
        static Logger& instance();
        std::string build_message(const Source& source, const char* format, ...);
        template<typename... Args>
        void trace(TraceSeverity severity, const Source& source, const char* format, Args... args)
        {
            std::string message = build_message(source, format, args...);
            log(severity, message.c_str());
        }
        void log(TraceSeverity severity, const char* message);
        void flush();
        TraceSeverity get_severity() const { return m_severity; }
    
    private:
        Logger() = default;
        
    private:
        TraceSeverity m_severity;
        static const int LocalBufferSize = 2000;
    };
    
    
    inline std::string Logger::build_message(const Source& source, const char* format, ...)
    {
        va_list arguments;
        va_start(arguments, format);
        std::string result;
        
        char buf[LocalBufferSize] = "";
        int size = snprintf(buf, LocalBufferSize, "%s:%s:%d    ", source.file, source.function, source.line);
        assert(size >= 0 && size < LocalBufferSize);
        int tempSize = vsnprintf(buf + size, LocalBufferSize - size, format, arguments);
        assert(tempSize >= 0);
        size += tempSize;
        
        if(size < LocalBufferSize)
            result = buf;
        else //message was trunced or operation failed
        {
            int bufferSize = std::max<int>(size, 32 * 1024);
            std::vector<char> largerBuf;
            largerBuf.resize(bufferSize);
            int largerSize = snprintf(&largerBuf[0], bufferSize, "%s:%s:%d\t", source.file, source.function, source.line);
            assert(largerSize >= 0 && largerSize < bufferSize);
            int remainSize = vsnprintf(&largerBuf[largerSize], bufferSize - largerSize, format, arguments);
            assert(remainSize >= 0); //We will print what we can, no second resize.
            result = std::string(largerBuf.begin(), largerBuf.end());
        }
        
        va_end(arguments);
        return result;
    }

#define TRACE_IMPL(severity, ...)\
    if(severity >= utils::Logger::instance().get_severity()) \
        utils::Logger::instance().trace(severity, __UTILS_SOURCE, __VA_ARGS__)

#define TRACE_ERROR(...) \
    TRACE_IMPL(utils::TraceSeverity::Fatal, __VA_ARGS__)
#define TRACE_INFO(...) \
    TRACE_IMPL(utils::TraceSeverity::Info, __VA_ARGS__)
#define TRACE_VERBOSE(...) \
    TRACE_IMPL(utils::TraceSeverity::Verbose, __VA_ARGS__)
}