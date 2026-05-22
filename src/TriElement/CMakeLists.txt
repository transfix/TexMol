cmake_minimum_required(VERSION 3.5)

SET(NAME "TriElement")

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/${NAME}/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  ../../inc/${NAME}
  ../../inc/
)

#ADD_SUBDIRECTORY(TestTriElement)

ADD_LIBRARY(TriaElement STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

SetupPETSC(TriaElement)
