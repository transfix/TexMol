cmake_minimum_required(VERSION 3.5)

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/InterfaceStats/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${INCLUDE_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
)

SET(IS_LINK_LIBS InterfaceStats Blurmaps GOAFileTypes PDBParser DPG UsefulMath Utility) # lapack)
#SET(DAG_LINK_LIBS PDBParser DPG DynamicAdaptiveGrid)


# arand: find_package(LAPACK) only works with Fortran codes...
#find_package(LAPACK)
#if(LAPACK_FOUND)
#  set(IS_LINK_LIBS ${IS_LINK_LIBS} ${LAPACK_LIBRARIES})
#  set(CMAKE_LD_FLAGS ${CMAKE_LD_FLAGS} ${LAPACK_LINKER_FLAGS})
#else(LAPACK_FOUND)
#  message(SEND_ERROR "InterfaceStats requires LAPACK!")
#endif(LAPACK_FOUND)

ADD_LIBRARY (InterfaceStats STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

SetupLapack(InterfaceStats)

#option(BUILD_INTERFACESTATS_STANDALONE "Enable building of InterfaceStats standalone utils" OFF)
#if(BUILD_INTERFACESTATS_STANDALONE)

ADD_EXECUTABLE (testInterfaceStats testInterfaceStats.cpp)
TARGET_LINK_LIBRARIES(testInterfaceStats ${IS_LINK_LIBS})

ADD_EXECUTABLE (testCapri testCapri.cpp)
TARGET_LINK_LIBRARIES(testCapri ${IS_LINK_LIBS})

ADD_EXECUTABLE (computeAllCapriScores computeAllCapriScores.cpp)
TARGET_LINK_LIBRARIES(computeAllCapriScores ${IS_LINK_LIBS})

#endif(BUILD_INTERFACESTATS_STANDALONE)
