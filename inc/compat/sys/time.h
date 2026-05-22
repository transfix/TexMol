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

/* struct timeval is defined in <winsock2.h>, but pulling in the
 * full Winsock stack can cause redefinition warnings.  Define a
 * minimal version if it hasn't been included yet.                */
#ifndef _WINSOCK2API_
#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED
struct timeval {
    long tv_sec;
    long tv_usec;
};
#endif /* _TIMEVAL_DEFINED */
#endif /* _WINSOCK2API_ */

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

#include <windows.h>

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
