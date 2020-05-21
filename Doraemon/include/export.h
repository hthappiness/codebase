
#pragma once

#if !defined(DORAEMON_EXPORT)

#if defined(DORAEMON_SHARED_LIBRARY)
#if defined(_WIN32)

#if defined(DORAEMON_COMPILE_LIBRARY)
#define DORAEMON_EXPORT __declspec(dllexport)
#else
#define DORAEMON_EXPORT __declspec(dllimport)
#endif  // defined(DORAEMON_COMPILE_LIBRARY)

#else  // defined(_WIN32)
#if defined(DORAEMON_COMPILE_LIBRARY)
#define DORAEMON_EXPORT __attribute__((visibility("default")))
#else
#define DORAEMON_EXPORT
#endif
#endif  // defined(_WIN32)

#else  // defined(DORAEMON_SHARED_LIBRARY)
#define DORAEMON_EXPORT
#endif

#endif  // !defined(DORAEMON_EXPORT)