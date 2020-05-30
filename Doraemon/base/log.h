

#pragma once

#define TC_INFO  1
#define TC_WARN  2
#define TC_ERROR 3

#define LOGI(format, ...) logger(TC_INFO, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define LOGW(format, ...) logger(TC_WARN, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define LOGE(format, ...) logger(TC_ERROR, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#include <strarg.h>

inline void logger(long lLevel, const char* fileName, long lLineNum, const char* funcName, const char* format, ...)
{
    static std::mutex Mutex;
    static char* Tempbuffer = nullptr;
    static char* buffer = nullptr;
    static auto logBufferSize = 1*1024*1024;
    static auto bufferSize = 2*logBufferSize;

    std::lock_guard<std::mutex> guard(Mutex);
    if( nullptr == Tempbuffer )
    {
        Tempbuffer = malloc(bufferSize);
        if( nullptr == Tempbuffer )
        {
            return ;
        }

        buffer = Tempbuffer + logBufferSize;
    }

    va_list args;
    va_start(args, format);
    vsnprintf(Tempbuffer, logBufferSize, format, args);
    va_end(args);

    snprintf(buffer, logBufferSize, "[%s:%ld @ %s] %s", fileName, lLineNum, funcName, Tempbuffer) ;

    LOG(INFO)<<"["<<buffer<<"]";
}