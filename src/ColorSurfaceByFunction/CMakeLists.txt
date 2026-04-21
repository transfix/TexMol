cmake_minimum_required(VERSION 3.5)

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/ColorSurfaceByFunction/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${INCLUDE_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
)

ADD_LIBRARY (ColorSurfaceByFunction STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

