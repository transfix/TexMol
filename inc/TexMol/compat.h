/*
 * TexMol portability shim — hides POSIX-vs-Win32 differences.
 *
 * Include this header instead of directly including:
 *   <pthread.h>, <unistd.h>, <sys/time.h>, <strings.h>
 *
 * It also provides:
 *   - strcasecmp / strncasecmp  (mapped to _stricmp / _strnicmp on MSVC)
 *   - sleep()                   (mapped to Sleep() on Windows)
 *   - gettimeofday()            (emulated on Windows)
 *   - ssize_t                   (typedef on MSVC)
 *   - drand48 / srand48         (emulated on Windows)
 *   - getline()                 (emulated on MSVC)
 */

#ifndef TEXMOL_COMPAT_H
#define TEXMOL_COMPAT_H

/* ────────────────────────────────────────────────────────────── */
/*  Headers                                                       */
/* ────────────────────────────────────────────────────────────── */
#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#  include <io.h>
#  include <process.h>
#  include <sys/timeb.h>
#else
#  include <unistd.h>
#  include <sys/time.h>
#  include <strings.h>  /* strcasecmp */
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/* ────────────────────────────────────────────────────────────── */
/*  strcasecmp / strncasecmp                                      */
/* ────────────────────────────────────────────────────────────── */
#ifdef _MSC_VER
#  ifndef strcasecmp
#    define strcasecmp  _stricmp
#  endif
#  ifndef strncasecmp
#    define strncasecmp _strnicmp
#  endif
#endif

/* ────────────────────────────────────────────────────────────── */
/*  ssize_t                                                       */
/* ────────────────────────────────────────────────────────────── */
#ifdef _MSC_VER
#  include <BaseTsd.h>
   typedef SSIZE_T ssize_t;
#endif

/* ────────────────────────────────────────────────────────────── */
/*  sleep (seconds) — inline function instead of macro to avoid   */
/*  interfering with QThread::sleep().                            */
/* ────────────────────────────────────────────────────────────── */
#ifdef _WIN32
static inline void sleep(unsigned int sec) { Sleep(sec * 1000); }
#endif

/* ────────────────────────────────────────────────────────────── */
/*  gettimeofday                                                  */
/* ────────────────────────────────────────────────────────────── */
#ifdef _WIN32
#  ifndef TEXMOL_GETTIMEOFDAY_DEFINED
#    define TEXMOL_GETTIMEOFDAY_DEFINED
/* With WIN32_LEAN_AND_MEAN, windows.h doesn't pull in winsock.h, so
   struct timeval is unavailable.  Define it ourselves. */
#    ifndef _WINSOCKAPI_   /* winsock.h already defines timeval */
struct timeval {
    long tv_sec;
    long tv_usec;
};
#    endif
static inline int gettimeofday(struct timeval *tv, void *tz)
{
    (void)tz;
    if (tv) {
        struct _timeb tb;
        _ftime_s(&tb);
        tv->tv_sec  = (long)tb.time;
        tv->tv_usec = tb.millitm * 1000;
    }
    return 0;
}
#  endif
#endif

/* ────────────────────────────────────────────────────────────── */
/*  drand48 / srand48                                             */
/* ────────────────────────────────────────────────────────────── */
#ifdef _WIN32
#  ifndef drand48
#    define drand48() ((double)rand() / RAND_MAX)
#  endif
#  ifndef srand48
#    define srand48(seed) srand((unsigned int)(seed))
#  endif
#endif

/* ────────────────────────────────────────────────────────────── */
/*  POSIX getline                                                 */
/* ────────────────────────────────────────────────────────────── */
#if defined(_MSC_VER) && !defined(TEXMOL_GETLINE_DEFINED)
#define TEXMOL_GETLINE_DEFINED
static inline ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
    if (!lineptr || !n || !stream) return -1;
    char *buf = *lineptr;
    size_t capacity = *n;
    size_t len = 0;
    int c;
    if (capacity == 0) {
        capacity = 128;
        buf = (char *)malloc(capacity);
        if (!buf) return -1;
    }
    while ((c = fgetc(stream)) != EOF) {
        if (len + 2 > capacity) {
            capacity *= 2;
            char *tmp = (char *)realloc(buf, capacity);
            if (!tmp) { free(buf); return -1; }
            buf = tmp;
        }
        buf[len++] = (char)c;
        if (c == '\n') break;
    }
    if (len == 0 && c == EOF) { *lineptr = buf; *n = capacity; return -1; }
    buf[len] = '\0';
    *lineptr = buf;
    *n = capacity;
    return (ssize_t)len;
}
#endif

#endif /* TEXMOL_COMPAT_H */
