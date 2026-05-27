cmake_minimum_required(VERSION 3.16)

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/AssemblyUtils/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
  ../../inc/AssemblyUtils
)

ADD_LIBRARY (AssemblyUtils STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
SetupLapack(AssemblyUtils)

# Test executables require pthreads (via fast-clash) and LAPACK — skip on Windows
if(NOT WIN32)
  SET(AU_LINK_LIBS AssemblyUtils Threads::Threads fast-clash GOAFileTypes PDBParser DPG UsefulMath Utility)

  ADD_EXECUTABLE (testAlignedOligomerFormation testAlignedOligomerFormation.cpp)
  TARGET_LINK_LIBRARIES(testAlignedOligomerFormation ${AU_LINK_LIBS})
  SetupLapack(testAlignedOligomerFormation)

  ADD_EXECUTABLE (testSymmetricPoseSet testSymmetricPoseSet.cpp)
  TARGET_LINK_LIBRARIES(testSymmetricPoseSet ${AU_LINK_LIBS})
endif()
