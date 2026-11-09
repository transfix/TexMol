/*
  Copyright 2026 CyberPC Angel, LLC

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

// MSVC force-include shim, the same fix as libcvc's inc/cvc/msvc_compat.h.
//
// On recent Windows SDKs the Universal CRT's <stdio.h> remaps legacy names
// textually (`#define snprintf _snprintf`), which turns Boost's
// `std::snprintf(...)` into `std::_snprintf(...)` and fails with C2039.
// CMakeLists.txt force-includes this header (/FI) on MSVC so that every
// translation unit undoes the remap before any third-party header.
// libcvc's own /FI only reaches libcvc's targets, not TexMol's.

#ifndef TEXMOL_MSVC_COMPAT_H
#define TEXMOL_MSVC_COMPAT_H

#if defined(_MSC_VER) && defined(__cplusplus)
#include <cstdarg>
#include <cstdio>
#ifdef snprintf
#undef snprintf
#endif
#ifdef vsnprintf
#undef vsnprintf
#endif
// If a name was rewritten before the undefs took effect, keep
// std::_snprintf / std::_vsnprintf resolvable.
namespace std {
using ::_snprintf;
using ::_vsnprintf;
} // namespace std
#endif

#endif // TEXMOL_MSVC_COMPAT_H
