cmake_minimum_required(VERSION 3.16)

if(WIN32)
  add_definitions(-DWIN32)
endif()

if(ENABLE_STEREO_DISPLAY)
  add_definitions(-DENABLE_STEREO_DISPLAY)
endif()

file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ../../inc/QGLViewer/*.[Hh])

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${INCLUDE_FILES})

set(UI_FORMS ImageInterface.ui)

option(USING_QGLVIEWER_VECTORIAL_RENDERER "Flag for enabling VRender code for QGLViewer" ON)
mark_as_advanced(USING_QGLVIEWER_VECTORIAL_RENDERER)
if(USING_QGLVIEWER_VECTORIAL_RENDERER)
  file(GLOB VRENDER_SOURCE_FILES VRender/*.cpp)
  file(GLOB VRENDER_INCLUDE_FILES ../../inc/QGLViewer/VRender/*.h)
  list(APPEND SOURCE_FILES ${VRENDER_SOURCE_FILES})
  list(APPEND INCLUDE_FILES ${VRENDER_INCLUDE_FILES})
  list(APPEND UI_FORMS VRenderInterface.ui)
else()
  add_definitions(-DNO_VECTORIAL_RENDER)
endif()

add_library(QGLViewer STATIC
  ${SOURCE_FILES}
  ${INCLUDE_FILES}
  ${UI_FORMS}
)

target_include_directories(QGLViewer PRIVATE
  ../../inc
  ${CMAKE_CURRENT_BINARY_DIR}  # for AUTOUIC-generated headers
)

target_link_libraries(QGLViewer
  Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets Qt6::Xml
  OpenGL::GL
  glew
)
