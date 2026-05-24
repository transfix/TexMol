cmake_minimum_required(VERSION 3.16)

FILE(GLOB SOURCE_FILES 
  ./*.cpp
  ./Sphere/*.cpp
)
FILE(GLOB INCLUDE_FILES 
  ../../inc/ObjectRenderer/*.[Hh]
  ../../inc/ObjectRenderer/Sphere/*.[Hh]
)

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${INCLUDE_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
)

set(LIBS "")

if(CG_FOUND)
  include_directories(
    ${CG_INCLUDE_PATH}
  )
  set(LIBS ${LIBS} ${CG_LIBRARY} ${CG_GL_LIBRARY})
endif(CG_FOUND)

add_library(ObjectRenderer STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(ObjectRenderer ${LIBS} GLEW::GLEW)
