cmake_minimum_required(VERSION 3.16)

SetupQt()

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/GeometryRenderer/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Include Files" FILES ${INCLUDE_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
)

set(LIBS "")

if(CG_FOUND)
  include_directories(
    ${CG_INCLUDE_PATH}
  )
  set(LIBS ${CG_LIBRARY} ${CG_GL_LIBRARY})
endif(CG_FOUND)

add_library(GeometryRenderer STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(GeometryRenderer ${LIBS} Qt6::Widgets)

#because we include ColorTable.h
SetupBoost(GeometryRenderer)
