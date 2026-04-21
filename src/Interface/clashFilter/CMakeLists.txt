cmake_minimum_required(VERSION 3.5)

FILE(GLOB SOURCE_FILES 
  ./*.cpp
)

FILE(GLOB QT_UI_FORMS 
  ./*.ui
)


FILE(GLOB INCLUDE_FILES 
  ../../../inc/Interface/clashFilter/*.[Hh]
)


SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  ../../../inc
  ${CMAKE_BINARY_DIR}/src/Interface/clashFilter
  ../../../inc/Interface/clashFilter
)

ADD_LIBRARY(clashFilter STATIC 
	${SOURCE_FILES}
	${INCLUDE_FILES}
)
