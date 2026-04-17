// Compatibility shim: forward to libcvc bounding_box (Phase 8)
#ifndef __TEXMOL_CVC_COMPAT_BOUNDINGBOX_H__
#define __TEXMOL_CVC_COMPAT_BOUNDINGBOX_H__

#include <cvc/bounding_box.h>

namespace CVC
{
  template <typename T>
  using GenericBoundingBox = cvc::generic_bounding_box<T>;

  typedef cvc::bounding_box       BoundingBox;
  typedef cvc::index_bounding_box IndexBoundingBox;

  using InvalidBoundingBox       = cvc::invalid_bounding_box;
  using InvalidBoundingBoxString = cvc::invalid_bounding_box_string;
}

#endif