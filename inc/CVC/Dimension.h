// Compatibility shim: forward to libcvc dimension (Phase 8)
#ifndef __TEXMOL_CVC_COMPAT_DIMENSION_H__
#define __TEXMOL_CVC_COMPAT_DIMENSION_H__

#include <cvc/dimension.h>

namespace CVC
{
  using Dimension = cvc::dimension;
}

#endif
