// Compatibility shim: forward to libcvc app (Phase 8)
// CVC::App, CVC::ThreadInfo, etc. → cvc::app, cvc::thread_info
#ifndef __TEXMOL_CVC_COMPAT_APP_H__
#define __TEXMOL_CVC_COMPAT_APP_H__

#include <cvc/app.h>

namespace CVC
{
  using App            = cvc::app;
  using ThreadInfo     = cvc::thread_info;
  using ThreadFeedback = cvc::thread_feedback;
  using ScopedLock     = cvc::scoped_lock;
}

// Legacy macro — if not already defined by libcvc's app.h
#ifndef cvcapp
#define cvcapp cvc::app::instance()
#endif

#endif
