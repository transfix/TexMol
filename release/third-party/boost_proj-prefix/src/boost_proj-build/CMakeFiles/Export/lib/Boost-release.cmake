#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
SET(CMAKE_IMPORT_FILE_VERSION 1)

# Compute the installation prefix relative to this file.
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)

# Import target "boost_date_time-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_date_time-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_date_time-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_date_time-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_date_time-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_date_time-mt-static "${_IMPORT_PREFIX}/lib/libboost_date_time-mt.a" )

# Import target "boost_date_time-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_date_time-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_date_time-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_date_time-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_date_time-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_date_time-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_date_time-mt-d.a" )

# Import target "boost_thread-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_thread-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_thread-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_thread-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_thread-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_thread-mt-static "${_IMPORT_PREFIX}/lib/libboost_thread-mt.a" )

# Import target "boost_thread-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_thread-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_thread-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_thread-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_thread-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_thread-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_thread-mt-d.a" )

# Import target "boost_regex-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_regex-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_regex-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_regex-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_regex-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_regex-mt-static "${_IMPORT_PREFIX}/lib/libboost_regex-mt.a" )

# Import target "boost_regex-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_regex-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_regex-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_regex-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_regex-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_regex-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_regex-mt-d.a" )

# Import target "boost_serialization-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_serialization-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_serialization-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_serialization-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_serialization-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_serialization-mt-static "${_IMPORT_PREFIX}/lib/libboost_serialization-mt.a" )

# Import target "boost_serialization-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_serialization-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_serialization-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_serialization-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_serialization-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_serialization-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_serialization-mt-d.a" )

# Import target "boost_wserialization-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_wserialization-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_wserialization-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt;boost_serialization-mt-static"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_wserialization-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_wserialization-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_wserialization-mt-static "${_IMPORT_PREFIX}/lib/libboost_wserialization-mt.a" )

# Import target "boost_wserialization-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_wserialization-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_wserialization-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt;boost_serialization-mt-static-debug"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_wserialization-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_wserialization-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_wserialization-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_wserialization-mt-d.a" )

# Import target "boost_system-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_system-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_system-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_system-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_system-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_system-mt-static "${_IMPORT_PREFIX}/lib/libboost_system-mt.a" )

# Import target "boost_system-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_system-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_system-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_system-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_system-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_system-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_system-mt-d.a" )

# Import target "boost_prg_exec_monitor-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_prg_exec_monitor-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_prg_exec_monitor-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_prg_exec_monitor-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_prg_exec_monitor-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_prg_exec_monitor-mt-static "${_IMPORT_PREFIX}/lib/libboost_prg_exec_monitor-mt.a" )

# Import target "boost_prg_exec_monitor-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_prg_exec_monitor-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_prg_exec_monitor-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_prg_exec_monitor-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_prg_exec_monitor-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_prg_exec_monitor-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_prg_exec_monitor-mt-d.a" )

# Import target "boost_test_exec_monitor-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_test_exec_monitor-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_test_exec_monitor-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_test_exec_monitor-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_test_exec_monitor-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_test_exec_monitor-mt-static "${_IMPORT_PREFIX}/lib/libboost_test_exec_monitor-mt.a" )

# Import target "boost_test_exec_monitor-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_test_exec_monitor-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_test_exec_monitor-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_test_exec_monitor-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_test_exec_monitor-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_test_exec_monitor-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_test_exec_monitor-mt-d.a" )

# Import target "boost_unit_test_framework-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_unit_test_framework-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_unit_test_framework-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_unit_test_framework-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_unit_test_framework-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_unit_test_framework-mt-static "${_IMPORT_PREFIX}/lib/libboost_unit_test_framework-mt.a" )

# Import target "boost_unit_test_framework-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_unit_test_framework-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_unit_test_framework-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_unit_test_framework-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_unit_test_framework-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_unit_test_framework-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_unit_test_framework-mt-d.a" )

# Import target "boost_filesystem-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_filesystem-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_filesystem-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt;boost_system-mt-static"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_filesystem-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_filesystem-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_filesystem-mt-static "${_IMPORT_PREFIX}/lib/libboost_filesystem-mt.a" )

# Import target "boost_filesystem-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_filesystem-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_filesystem-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt;boost_system-mt-static-debug"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_filesystem-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_filesystem-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_filesystem-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_filesystem-mt-d.a" )

# Import target "boost_iostreams-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_iostreams-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_iostreams-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "/usr/lib64/libz.so;/usr/lib64/libbz2.so;pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_iostreams-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_iostreams-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_iostreams-mt-static "${_IMPORT_PREFIX}/lib/libboost_iostreams-mt.a" )

# Import target "boost_iostreams-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_iostreams-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_iostreams-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "/usr/lib64/libz.so;/usr/lib64/libbz2.so;pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_iostreams-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_iostreams-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_iostreams-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_iostreams-mt-d.a" )

# Import target "boost_program_options-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_program_options-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_program_options-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_program_options-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_program_options-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_program_options-mt-static "${_IMPORT_PREFIX}/lib/libboost_program_options-mt.a" )

# Import target "boost_program_options-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_program_options-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_program_options-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_program_options-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_program_options-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_program_options-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_program_options-mt-d.a" )

# Import target "boost_signals-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_signals-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_signals-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_signals-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_signals-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_signals-mt-static "${_IMPORT_PREFIX}/lib/libboost_signals-mt.a" )

# Import target "boost_signals-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_signals-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_signals-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_signals-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_signals-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_signals-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_signals-mt-d.a" )

# Import target "boost_wave-mt-static" for configuration "Release"
SET_PROPERTY(TARGET boost_wave-mt-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_wave-mt-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt;boost_filesystem-mt-static;boost_thread-mt-static;boost_date_time-mt-static"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_wave-mt.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_wave-mt-static )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_wave-mt-static "${_IMPORT_PREFIX}/lib/libboost_wave-mt.a" )

# Import target "boost_wave-mt-static-debug" for configuration "Release"
SET_PROPERTY(TARGET boost_wave-mt-static-debug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(boost_wave-mt-static-debug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread;rt;boost_filesystem-mt-static-debug;boost_thread-mt-static-debug;boost_date_time-mt-static-debug"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libboost_wave-mt-d.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS boost_wave-mt-static-debug )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_boost_wave-mt-static-debug "${_IMPORT_PREFIX}/lib/libboost_wave-mt-d.a" )

# Loop over all imported files and verify that they actually exist
FOREACH(target ${_IMPORT_CHECK_TARGETS} )
  FOREACH(file ${_IMPORT_CHECK_FILES_FOR_${target}} )
    IF(NOT EXISTS "${file}" )
      MESSAGE(FATAL_ERROR "The imported target \"${target}\" references the file
   \"${file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
    ENDIF()
  ENDFOREACH()
  UNSET(_IMPORT_CHECK_FILES_FOR_${target})
ENDFOREACH()
UNSET(_IMPORT_CHECK_TARGETS)

# Cleanup temporary variables.
SET(_IMPORT_PREFIX)

# Commands beyond this point should not need to know the version.
SET(CMAKE_IMPORT_FILE_VERSION)
