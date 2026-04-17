// Compatibility shim: forward to libcvc hdf5_utils (Phase 8)
// Only active when CVC_USING_HDF5 is enabled.
#ifndef __TEXMOL_CVC_COMPAT_HDF5_UTILS_H__
#define __TEXMOL_CVC_COMPAT_HDF5_UTILS_H__

#ifdef CVC_USING_HDF5
#include <cvc/hdf5_utils.h>

namespace CVC
{
  using InvalidHDF5File = cvc::invalid_hdf5_file;
  using HDF5Exception   = cvc::hdf5_exception;

  namespace HDF5_Utilities = cvc::hdf5_utils;
}
#endif // CVC_USING_HDF5

#endif
