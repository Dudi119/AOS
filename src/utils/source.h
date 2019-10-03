#pragma once

#include <cstring>

namespace utils {
        
        struct Source {
            const char *file;
            const char *function;
            int line;
        };
}

#define __UTILS__FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define __UTILS_SOURCE utils::Source{__UTILS__FILENAME__, __FUNCTION__, __LINE__}
