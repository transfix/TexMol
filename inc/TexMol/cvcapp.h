#ifndef TEXMOL_CVCAPP_H
#define TEXMOL_CVCAPP_H

#include <cvc/app.h>

namespace texmol {
// Process-wide cvc::app instance for TexMol.  Created on first access.
inline cvc::app &app() {
  static cvc::app instance;
  return instance;
}
} // namespace texmol

#define cvcapp texmol::app()

#endif
