# Install script for directory: /workspace/cvcsoft/TexMol/third-party/hdf5

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/workspace/cvcsoft/TexMol/release")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "headers")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/H5pubconf.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "headers")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "configinstall")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5/hdf5-targets.cmake")
    FILE(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5/hdf5-targets.cmake"
         "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/CMakeFiles/Export/share/cmake/hdf5/hdf5-targets.cmake")
    IF(EXPORT_FILE_CHANGED)
      FILE(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5/hdf5-targets-*.cmake")
      IF(OLD_CONFIG_FILES)
        MESSAGE(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5/hdf5-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        FILE(REMOVE ${OLD_CONFIG_FILES})
      ENDIF(OLD_CONFIG_FILES)
    ENDIF(EXPORT_FILE_CHANGED)
  ENDIF()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5" TYPE FILE FILES "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/CMakeFiles/Export/share/cmake/hdf5/hdf5-targets.cmake")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5" TYPE FILE FILES "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/CMakeFiles/Export/share/cmake/hdf5/hdf5-targets-noconfig.cmake")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "configinstall")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "configinstall")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5" TYPE FILE FILES "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/CMakeFiles/FindHDF5.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "configinstall")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "configinstall")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5" TYPE FILE FILES "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/CMakeFiles/hdf5-config.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "configinstall")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "configinstall")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5" TYPE FILE FILES "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/CMakeFiles/hdf5-config-version.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "configinstall")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libraries")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/hdf5" TYPE FILE FILES "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/libhdf5.settings")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libraries")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "hdfdocuments")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share" TYPE FILE FILES
    "/workspace/cvcsoft/TexMol/third-party/hdf5/ACKNOWLEDGMENTS"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/COPYING"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/README.txt"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "hdfdocuments")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "hdfdocuments")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/release_docs" TYPE FILE FILES
    "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/CMake.txt"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/USING_CMake.txt"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/COPYING"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/HISTORY-1_8.txt"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/INSTALL"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/RELEASE.txt"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/INSTALL_Cygwin.txt"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/INSTALL_MinGW.txt"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/release_docs/INSTALL_VMS.txt"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "hdfdocuments")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/src/cmake_install.cmake")
  INCLUDE("/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/c++/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
