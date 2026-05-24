
cmake_minimum_required(VERSION 3.16)
FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/ImposterRenderer/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${INCLUDE_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
)

set(LIBS GLEW::GLEW)

option(USING_AO "option for using ambient occusion for ball rendering" ON)
if(USING_AO)
  add_definitions(-DAO)
endif(USING_AO)

if(CG_FOUND)
  include_directories(
    ${CG_INCLUDE_PATH}
  )
  set(LIBS ${LIBS} ${CG_LIBRARY} ${CG_GL_LIBRARY})
endif(CG_FOUND)

add_library(ImposterRenderer STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(ImposterRenderer ${LIBS})
