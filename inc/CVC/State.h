// Compatibility shim: forward to libcvc state (Phase 8)
// CVC::State → cvc::state
#ifndef __TEXMOL_CVC_COMPAT_STATE_H__
#define __TEXMOL_CVC_COMPAT_STATE_H__

#include <cvc/state.h>

namespace CVC
{
  using State = cvc::state;
}

// Legacy macro — if not already defined by libcvc's state.h
#ifndef cvcstate
#define cvcstate cvc::state::instance()
#endif

#endif
