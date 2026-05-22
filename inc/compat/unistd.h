/*
 * Windows compatibility shim for <unistd.h>.
 *
 * On POSIX systems this header is never reached because the real
 * <unistd.h> is found first.  On Windows (MSVC) the inc/compat/
 * directory is added to the include path so the compiler can
 * resolve #include <unistd.h> to this stub.
 */
#ifndef TEXMOL_COMPAT_UNISTD_H
#define TEXMOL_COMPAT_UNISTD_H

#ifdef _WIN32

#include <io.h>
#include <process.h>
#include <direct.h>

/* Common POSIX → MSVC mappings */
#ifndef access
#define access _access
#endif
#ifndef R_OK
#define R_OK 4
#define W_OK 2
#define F_OK 0
#endif

#endif /* _WIN32 */
#endif /* TEXMOL_COMPAT_UNISTD_H */
