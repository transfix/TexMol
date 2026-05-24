cmake_minimum_required(VERSION 3.16)

SET(NAME "KIFMM3D/fmm3d")

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/../../../inc/${NAME}/*.[Hh] ${CMAKE_SOURCE_DIR}/../../../inc/${NAME}/*.hpp)

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

#INCLUDE_DIRECTORIES(/usr/include)
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../../inc/KIFMM3D)
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../../inc/KIFMM3D/common)
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../inc/${NAME})
#INCLUDE_DIRECTORIES(${PETSC_DIR}/${PETSC_ARCH}/include)
#INCLUDE_DIRECTORIES(${PETSC_DIR}/include)
#INCLUDE_DIRECTORIES(${TAU_INCLUDE})
#INCLUDE_DIRECTORIES(${PETSC_BLASLAPACK_FLAGS})


INCLUDE_DIRECTORIES(
  ../../../inc
  ../../../inc/KIFMM3D/
  ../../../inc/KIFMM3D/common
  ../../../inc/KIFMM3D/fmm3d
  $ENV{PETSC_DIR}/${PETSC_ARCH}/include
  $ENV{PETSC_DIR}/include
  /usr/include
#  $ENV{TAU_INCLUDE}
#  $ENV{PETSC_BLASLAPACK_FLAGS}
)

ADD_LIBRARY (KIFMM3D STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

SetupPETSC(KIFMM3D)
SetupFFTW(KIFMM3D)

#ADD_EXECUTABLE(${NAME} STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

#TARGET_LINK_LIBRARIES(${NAME}
#   fmm3d
#   common
#   petsc
#   petscvec
#   petscmat
#   fftw3
#   lapack
#   atlas
#   cblas
#   dl
#   gcc_s
#   gfortranbegin
#   gfortran
#   m
#   f77blas
#)
