// Phase 5 compatibility shim: LBIE_lib/Geoframe.h → cvc/geometry.h
// The original Geoframe was part of the LBIE meshing library.
// In libcvc, mesh geometry is now represented by cvc::geometry.
#ifndef __TEXMOL_GEOFRAME_COMPAT_H__
#define __TEXMOL_GEOFRAME_COMPAT_H__
#include <cvc/geometry.h>
#endif
