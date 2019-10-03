#include "logger.h"

namespace utils{
    Logger& Logger::instance()
    {
        static Logger logger;
        return logger;
    }
    
    void Logger::log(utils::TraceSeverity severity, const char *message)
    {
        printf("%s", message);
    }
    
    void Logger::flush() {}
}
