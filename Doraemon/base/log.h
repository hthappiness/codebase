

#pragma once

#define TC_INFO  1
#define TC_WARN  2
#define TC_ERROR 3

#define LOGI(format, ...) logger(TC_INFO, format, ##__VA_ARGS__)
#define LOGW(format, ...) logger(TC_WARN, format, ##__VA_ARGS__)
#define LOGE(format, ...) logger(TC_ERROR, format, ##__VA_ARGS__)

#include <strarg.h>

inline void logger(long lLevel, const char* format, ...)
{
    static std::mutex Mutex;
    static char* buffer = nullptr;
    static auto bufferSize = 1*1024*1024;

    std::lock_guard<std::mutex> guard(Mutex);
    if( nullptr == buffer )
    {
        buffer = malloc(bufferSize);
    }

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, bufferSize, format, args);

    LOG(INFO)<<"["<<buffer<<"]";
    va_end(args);
}