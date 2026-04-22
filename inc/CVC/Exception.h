// Compatibility shim: TexMol-specific PascalCase exceptions (Phase 8)
// These are separate from libcvc's snake_case exceptions.
// Kept standalone to preserve the std::exception hierarchy that VolMagick depends on.
#ifndef __TEXMOL_CVC_COMPAT_EXCEPTIONS_H__
#define __TEXMOL_CVC_COMPAT_EXCEPTIONS_H__

#include <exception>
#include <string>

#ifndef CVC_COMPAT_EXCEPTION_DEFINED
#define CVC_COMPAT_EXCEPTION_DEFINED
namespace CVC
{
  class Exception : public std::exception
  {
  public:
    Exception() {}
    virtual ~Exception() noexcept {}
    virtual const std::string& what_str() const throw () = 0;
    virtual const char *what () const noexcept
    {
      return what_str().c_str();
    }
  };
}

// Use a distinct macro name to avoid collisions with libcvc's CVC_DEF_EXCEPTION
#define CVC_COMPAT_DEF_EXCEPTION(name) \
  class name : public CVC::Exception \
  { \
  public: \
    name () : _msg("CVC::"#name) {} \
    name (const std::string& msg) : \
      _msg(std::string("CVC::" #name " exception: ") + msg) {} \
    virtual ~name() noexcept {} \
    virtual const std::string& what_str() const noexcept { return _msg; } \
  private: \
    std::string _msg; \
  }

namespace CVC
{
  CVC_COMPAT_DEF_EXCEPTION(ReadError);
  CVC_COMPAT_DEF_EXCEPTION(WriteError);
  CVC_COMPAT_DEF_EXCEPTION(MemoryAllocationError);
  CVC_COMPAT_DEF_EXCEPTION(SubVolumeOutOfBounds);
  CVC_COMPAT_DEF_EXCEPTION(UnsupportedVolumeFileType);
  CVC_COMPAT_DEF_EXCEPTION(UnsupportedGeometryFileType);
  CVC_COMPAT_DEF_EXCEPTION(IndexOutOfBounds);
  CVC_COMPAT_DEF_EXCEPTION(NullDimension);
  CVC_COMPAT_DEF_EXCEPTION(VolumePropertiesMismatch);
  CVC_COMPAT_DEF_EXCEPTION(VolumeCacheDirectoryFileError);
  CVC_COMPAT_DEF_EXCEPTION(NetworkError);
  CVC_COMPAT_DEF_EXCEPTION(XmlRpcServerTerminate);
}
#endif // CVC_COMPAT_EXCEPTION_DEFINED

#endif

