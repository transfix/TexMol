cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ../../inc/ColorTable2/*.[Hh])

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${INCLUDE_FILES})

add_library(ColorTable2 STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(ColorTable2 PRIVATE ../../inc)
target_link_libraries(ColorTable2 Qt6::Widgets Qt6::OpenGLWidgets)

#Make sure we've got boost
SetupBoost(ColorTable2)
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
##find_package(Boost 1.34.0)
#find_package(Boost 1.41.0)
#if(Boost_FOUND)
#  include_directories(${Boost_INCLUDE_DIRS})
#  message("Boost includes: ${Boost_INCLUDE_DIRS}")
#else(Boost_FOUND)
#  message("ColorTable2: If you're having trouble finding boost, set environment variables "
#          "BOOST_INCLUDEDIR and BOOST_LIBRARYDIR to the appropriate paths")
#endif(Boost_FOUND)

# Contour tree and contour spectrum re-enabled (modules restored)
# add_compile_definitions(COLORTABLE2_DISABLE_CONTOUR_TREE COLORTABLE2_DISABLE_CONTOUR_SPECTRUM)

set(LIBS QGLViewer)

TARGET_LINK_LIBRARIES(ColorTable2 ${LIBS})
