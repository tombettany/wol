#ifndef __WOL_LOG_H__
#define __WOL_LOG_H__

#include <stdio.h>

enum log_levels {
    CRITICAL = 1,
    ERROR = 2,
    WARN = 3,
    INFO = 4,
    DEBUG = 5
};

#define LOG_LEVEL    INFO

#define LOG(log_level, ...) if (log_level <= LOG_LEVEL) printf(__VA_ARGS__)

#endif
