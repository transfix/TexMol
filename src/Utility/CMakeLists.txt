cmake_minimum_required(VERSION 3.5)

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/Utility/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
)

ADD_LIBRARY (Utility STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(Utility PUBLIC Threads::Threads)
