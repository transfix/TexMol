# Install script for directory: /workspace/cvcsoft/TexMol/third-party/hdf5/c++/src

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

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "cppheaders")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cpp" TYPE FILE FILES
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5AbstractDs.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5Alltypes.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5ArrayType.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5AtomType.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5Attribute.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5Classes.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5CommonFG.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5CompType.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5Cpp.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5CppDoc.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5DataSet.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5DataSpace.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5DataType.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5DcreatProp.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5DxferProp.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5EnumType.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5Exception.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5FaccProp.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5FcreatProp.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5File.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5FloatType.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5Group.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5IdComponent.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5Include.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5IntType.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5Library.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5Object.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5PredType.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5PropList.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5StrType.h"
    "/workspace/cvcsoft/TexMol/third-party/hdf5/c++/src/H5VarLenType.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "cppheaders")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "cpplibraries")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/bin/libhdf5_cpp.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "cpplibraries")

