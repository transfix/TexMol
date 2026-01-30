#-----------------------------------------------------------------------------
# HDF5 Config file for compiling against hdf5 build directory
#-----------------------------------------------------------------------------
GET_FILENAME_COMPONENT (SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

#-----------------------------------------------------------------------------
# User Options
#-----------------------------------------------------------------------------
SET (HDF5_ENABLE_PARALLEL OFF)
SET (HDF5_BUILD_FORTRAN   OFF)
SET (HDF5_ENABLE_F2003    )
SET (HDF5_BUILD_CPP_LIB   ON)
SET (HDF5_BUILD_TOOLS     OFF)
SET (HDF5_BUILD_HL_LIB    OFF)
SET (HDF5_ENABLE_Z_LIB_SUPPORT OFF)
SET (HDF5_ENABLE_SZIP_SUPPORT  OFF)
SET (HDF5_ENABLE_SZIP_ENCODING )
SET (HDF5_BUILD_SHARED_LIBS    FALSE)

#-----------------------------------------------------------------------------
# Directories
#-----------------------------------------------------------------------------
SET (HDF5_INCLUDE_DIR "/workspace/cvcsoft/TexMol/third-party/hdf5/src;/workspace/cvcsoft/TexMol/third-party/hdf5/c++;/workspace/cvcsoft/TexMol/third-party/hdf5/hl;/workspace/cvcsoft/TexMol/third-party/hdf5/tools;/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build")

IF (HDF5_BUILD_FORTRAN)
  SET (HDF5_INCLUDE_DIR_FORTRAN "/workspace/cvcsoft/TexMol/release/third-party/hdf5_proj-prefix/src/hdf5_proj-build/bin/fortran" )
ENDIF (HDF5_BUILD_FORTRAN)
  
IF (HDF5_BUILD_CPP_LIB)
  SET (HDF5_INCLUDE_DIR_CPP ${HDF5_INCLUDE_DIR} )
ENDIF (HDF5_BUILD_CPP_LIB)

IF (HDF5_BUILD_HL_LIB)
  SET (HDF5_INCLUDE_DIR_HL ${HDF5_INCLUDE_DIR} )
ENDIF (HDF5_BUILD_HL_LIB)

IF (HDF5_BUILD_HL_LIB AND HDF5_BUILD_CPP_LIB)
  SET (HDF5_INCLUDE_DIR_HL_CPP ${HDF5_INCLUDE_DIR} )
ENDIF (HDF5_BUILD_HL_LIB AND HDF5_BUILD_CPP_LIB)

IF (HDF5_BUILD_TOOLS)
  SET (HDF5_INCLUDE_DIR_TOOLS ${HDF5_INCLUDE_DIR} )
ENDIF (HDF5_BUILD_TOOLS)

IF (HDF5_BUILD_SHARED_LIBS)
  SET (H5_BUILT_AS_DYNAMIC_LIB 1 )
ELSE (HDF5_BUILD_SHARED_LIBS)
  SET (H5_BUILT_AS_STATIC_LIB 1 )
ENDIF (HDF5_BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Version Strings
#-----------------------------------------------------------------------------
SET (HDF5_VERSION_STRING 1.8.10)
SET (HDF5_VERSION_MAJOR  1.8)
SET (HDF5_VERSION_MINOR  10)

#-----------------------------------------------------------------------------
# Don't include targets if this file is being picked up by another
# project which has already build hdf5 as a subproject
#-----------------------------------------------------------------------------
IF (NOT TARGET "hdf5")
  INCLUDE (${SELF_DIR}/hdf5-targets.cmake)
  SET (HDF5_LIBRARIES "hdf5;hdf5_cpp")
ENDIF (NOT TARGET "hdf5")
