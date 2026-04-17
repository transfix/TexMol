// Compatibility shim: forward to libcvc state_object (Phase 8)
#ifndef __TEXMOL_CVC_COMPAT_STATE_OBJECT_H__
#define __TEXMOL_CVC_COMPAT_STATE_OBJECT_H__

#include <cvc/state_object.h>

namespace CVC
{
  template <class Derived>
  using StateObject = cvc::state_object<Derived>;
}

#endif
