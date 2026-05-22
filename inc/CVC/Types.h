// Compatibility shim: forward to libcvc types (Phase 8)
// Provides CVC:: PascalCase aliases for cvc:: snake_case types.
#ifndef __TEXMOL_CVC_COMPAT_TYPES_H__
#define __TEXMOL_CVC_COMPAT_TYPES_H__

#include <cvc/types.h>

namespace CVC
{
  // Bring all cvc:: names into CVC:: (enum values UChar, UShort, etc.)
  using namespace cvc;

  // PascalCase aliases for snake_case types
  typedef cvc::data_type                  DataType;
#ifndef CVC_COMPAT_TYPES_STATICS_DEFINED
#define CVC_COMPAT_TYPES_STATICS_DEFINED
  static const unsigned int*              DataTypeSizes = cvc::data_type_sizes;
  static const char**                     DataTypeStrings = cvc::data_type_strings;
#endif

  typedef cvc::locale_bool                LocaleBool;
  typedef cvc::signal                     Signal;
  typedef cvc::map_change_signal          MapChangeSignal;
  typedef cvc::data_map                   DataMap;
  typedef cvc::data_type_name_map         DataTypeNameMap;
  typedef cvc::data_type_enum_map         DataTypeEnumMap;
  typedef cvc::property_map               PropertyMap;
  typedef cvc::thread_ptr                 ThreadPtr;
  typedef cvc::thread_map                 ThreadMap;
  typedef cvc::thread_progress_map        ThreadProgressMap;
  typedef cvc::thread_key_map             ThreadKeyMap;
  typedef cvc::thread_info_map            ThreadInfoMap;
  typedef cvc::data_reader                DataReader;
  typedef cvc::data_reader_collection     DataReaderCollection;
  typedef cvc::mutex_ptr                  MutexPtr;
  typedef cvc::mutex_map_element          MutexMapElement;
  typedef cvc::mutex_map                  MutexMap;
}

#endif
