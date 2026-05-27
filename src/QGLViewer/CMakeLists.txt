# ===========================================================================
# QGLViewer v3.0.0 — from volrover (native QOpenGLWidget / Qt6)
# Replaces TexMol's ancient v2.3.6 (2008, QGLWidget-based)
# ===========================================================================
cmake_minimum_required(VERSION 3.16)

if(WIN32)
  add_definitions(-DWIN32 -DNOMINMAX)
  if(MSVC)
    add_compile_options(/EHsc /FS)
  endif()
endif()

if(ENABLE_STEREO_DISPLAY)
  add_definitions(-DENABLE_STEREO_DISPLAY)
endif()

# QGLViewer headers
set(QGL_HEADERS
  ../../inc/QGLViewer/qglviewer.h
  ../../inc/QGLViewer/camera.h
  ../../inc/QGLViewer/manipulatedFrame.h
  ../../inc/QGLViewer/manipulatedCameraFrame.h
  ../../inc/QGLViewer/frame.h
  ../../inc/QGLViewer/constraint.h
  ../../inc/QGLViewer/keyFrameInterpolator.h
  ../../inc/QGLViewer/mouseGrabber.h
  ../../inc/QGLViewer/quaternion.h
  ../../inc/QGLViewer/vec.h
  ../../inc/QGLViewer/domUtils.h
  ../../inc/QGLViewer/config.h
)

# QGLViewer sources
set(SOURCES
  qglviewer.cpp
  camera.cpp
  manipulatedFrame.cpp
  manipulatedCameraFrame.cpp
  frame.cpp
  saveSnapshot.cpp
  constraint.cpp
  keyFrameInterpolator.cpp
  mouseGrabber.cpp
  quaternion.cpp
  vec.cpp
)

set(UI_FORMS ImageInterface.ui)

# VRender vectorial rendering support
option(NO_VECTORIAL_RENDER "Disable vectorial rendering support" OFF)
if(NOT NO_VECTORIAL_RENDER)
  set(VRENDER_SOURCES
    VRender/BackFaceCullingOptimizer.cpp
    VRender/BSPSortMethod.cpp
    VRender/EPSExporter.cpp
    VRender/Exporter.cpp
    VRender/FIGExporter.cpp
    VRender/gpc.cpp
    VRender/ParserGL.cpp
    VRender/Primitive.cpp
    VRender/PrimitivePositioning.cpp
    VRender/TopologicalSortMethod.cpp
    VRender/VisibilityOptimizer.cpp
    VRender/Vector2.cpp
    VRender/Vector3.cpp
    VRender/NVector3.cpp
    VRender/VRender.cpp
  )
  set(VRENDER_HEADERS
    ../../inc/QGLViewer/VRender/AxisAlignedBox.h
    ../../inc/QGLViewer/VRender/Exporter.h
    ../../inc/QGLViewer/VRender/gpc.h
    ../../inc/QGLViewer/VRender/NVector3.h
    ../../inc/QGLViewer/VRender/Optimizer.h
    ../../inc/QGLViewer/VRender/ParserGL.h
    ../../inc/QGLViewer/VRender/Primitive.h
    ../../inc/QGLViewer/VRender/PrimitivePositioning.h
    ../../inc/QGLViewer/VRender/SortMethod.h
    ../../inc/QGLViewer/VRender/Types.h
    ../../inc/QGLViewer/VRender/Vector2.h
    ../../inc/QGLViewer/VRender/Vector3.h
    ../../inc/QGLViewer/VRender/VRender.h
  )
  list(APPEND SOURCES ${VRENDER_SOURCES})
  list(APPEND QGL_HEADERS ${VRENDER_HEADERS})
  list(APPEND UI_FORMS VRenderInterface.ui)
else()
  add_definitions(-DNO_VECTORIAL_RENDER)
endif()

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${QGL_HEADERS})
source_group("UI Files" FILES ${UI_FORMS})

# Static library for TexMol (volrover uses shared by default)
add_library(QGLViewer STATIC ${SOURCES} ${QGL_HEADERS} ${UI_FORMS})
target_compile_definitions(QGLViewer PUBLIC QGLVIEWER_STATIC)

set_target_properties(QGLViewer PROPERTIES
  CXX_STANDARD ${CMAKE_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON
  POSITION_INDEPENDENT_CODE ON
)

target_include_directories(QGLViewer
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../inc>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(QGLViewer
  PUBLIC
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Xml Qt6::OpenGL Qt6::OpenGLWidgets
)

# OpenGL / GLU
find_package(OpenGL REQUIRED)
if(WIN32)
  target_link_libraries(QGLViewer PUBLIC opengl32 glu32)
elseif(APPLE)
  target_link_libraries(QGLViewer PUBLIC "-framework OpenGL")
else()
  target_link_libraries(QGLViewer PUBLIC OpenGL::GL OpenGL::GLU)
endif()

# GLEW (optional — v3.0.0 doesn't require it in headers)
if(TARGET GLEW::GLEW)
  target_link_libraries(QGLViewer PUBLIC GLEW::GLEW)
elseif(TARGET glew)
  target_link_libraries(QGLViewer PUBLIC glew)
endif()
