#include "System.h"

#include <stddef.h>

#ifdef __linux__
#include <sys/time.h>
#elif _WIN32
#include <windows.h>
#endif

uint64_t systemGetCurrentMilliseconds() {
#ifdef __linux__
    struct timeval time;
    gettimeofday(&time, NULL);
    return (uint64_t) (time.tv_sec * 1000 + time.tv_usec / 1000);
#elif _WIN32
    FILETIME time;
    GetSystemTimeAsFileTime(&time);
    return ((LONGLONG) time.dwLowDateTime + ((LONGLONG) (time.dwHighDateTime) << 32LL)) / 10000;
#else
    return 0;
#endif
}
