# Install script for directory: /workspace/cvcsoft/TexMol/third-party/hdf5/src

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/hdf5.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5api_adpt.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5public.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5version.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5overflow.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Apkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Apublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5ACpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5ACpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5B2pkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5B2public.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Bpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Bpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Dpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Dpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Edefin.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Einit.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Epkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Epubgen.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Epublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Eterm.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Fpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Fpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDcore.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDdirect.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDfamily.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDlog.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDmpi.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDmpio.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDmpiposix.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDmulti.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDsec2.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDstdio.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FSpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FSpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Gpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Gpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HFpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HFpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HGpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HGpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HLpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HLpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5MPpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Opkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Opublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Oshared.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Ppkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Ppublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Spkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Spublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5SMpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Tpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Tpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Zpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Zpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Cpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Cpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Ipkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Ipublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Lpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Lpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5MMpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Rpkg.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Rpublic.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5private.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Aprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5ACprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5B2private.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Bprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5CSprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Dprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Eprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FDprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Fprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FLprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FOprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5MFprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5MMprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Cprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5FSprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Gprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HFprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HGprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HLprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5HPprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Iprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Lprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5MPprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Oprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Pprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5RCprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Rprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5RSprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5SLprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5SMprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Sprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5STprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Tprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5TSprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Vprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5WBprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5Zprivate.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/src/H5win32defs.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "headers")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libraries")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/bin/libhdf5.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libraries")

