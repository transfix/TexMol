CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/VirusAssembly/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
  ../../inc/VirusAssembly
)

ADD_LIBRARY (VirusAssembly STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

SET(VA_LINK_LIBS VirusAssembly AssemblyUtils pthread fast-clash GOAFileTypes PDBParser DPG UsefulMath Utility)

ADD_EXECUTABLE (testAssembleVirus testAssembleVirus.cpp)
TARGET_LINK_LIBRARIES(testAssembleVirus ${VA_LINK_LIBS})

ADD_EXECUTABLE (testLayout testLayout.cpp)
TARGET_LINK_LIBRARIES(testLayout ${VA_LINK_LIBS})
