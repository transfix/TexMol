cmake_minimum_required(VERSION 3.16)

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/VirusAssembly/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
  ../../inc/VirusAssembly
)

ADD_LIBRARY (VirusAssembly STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

# Test executables require pthreads (via fast-clash) and LAPACK — skip on Windows
if(NOT WIN32)
  SET(VA_LINK_LIBS VirusAssembly AssemblyUtils Threads::Threads fast-clash GOAFileTypes PDBParser DPG UsefulMath Utility)

  ADD_EXECUTABLE (testAssembleVirus testAssembleVirus.cpp)
  TARGET_LINK_LIBRARIES(testAssembleVirus ${VA_LINK_LIBS})

  ADD_EXECUTABLE (testLayout testLayout.cpp)
  TARGET_LINK_LIBRARIES(testLayout ${VA_LINK_LIBS})
endif()
