cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ../../inc/Histogram/*.[Hh])

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${INCLUDE_FILES})

add_library(Histogram STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(Histogram PRIVATE ../../inc)
target_link_libraries(Histogram Qt6::Widgets Qt6::OpenGLWidgets)
