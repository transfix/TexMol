/*
 * Windows compatibility shim for <sys/time.h>.
 *
 * Provides struct timeval and gettimeofday() on Windows where
 * the POSIX header does not exist.
 */
#ifndef TEXMOL_COMPAT_SYS_TIME_H
#define TEXMOL_COMPAT_SYS_TIME_H

#ifdef _WIN32

#include <time.h>
#include <windows.h>

/* struct timeval is provided by <winsock2.h>. If Winsock hasn't
 * been included, define it ourselves.                            */
#ifndef _WINSOCKAPI_
#ifndef _WINSOCK2API_
struct timeval {
    long tv_sec;
    long tv_usec;
};
#endif
#endif

#ifndef _TIMEZONE_DEFINED
#define _TIMEZONE_DEFINED
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};
#endif

static inline int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
    FILETIME ft;
    unsigned long long t;
    GetSystemTimeAsFileTime(&ft);
    t = ((unsigned long long)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    /* Convert from 100-ns intervals since 1601 to microseconds since 1970 */
    t -= 116444736000000000ULL;
    t /= 10;
    if (tp) {
        tp->tv_sec  = (long)(t / 1000000ULL);
        tp->tv_usec = (long)(t % 1000000ULL);
    }
    (void)tzp;
    return 0;
}

#endif /* _WIN32 */

#endif /* TEXMOL_COMPAT_SYS_TIME_H */
