# ===========================================================================
# F2DockClient — Qt6/AUTOMOC modernized (Phase 3)
# ===========================================================================
cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCE_FILES ./*.cpp)

# Exclude standalone executables from library sources
list(REMOVE_ITEM SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/GetResults.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CheckStatus.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/SubmitDockingJob.cpp
)

if(BUILD_INTERFACESTATS)
  add_definitions(-DWITH_INTERFACE_STATS)
endif()

# .ui files (renamed from .Qt4.ui in Phase 3)
file(GLOB UI_FORMS ./*.ui)

file(GLOB INCLUDE_FILES
  ../../inc/F2DockClient/*.[Hh]
)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("UI Files" FILES ${UI_FORMS})
source_group("Header Files" FILES ${INCLUDE_FILES})

include_directories(
  ../../inc
  ${CMAKE_CURRENT_BINARY_DIR}   # for uic-generated headers
  ../../inc/F2DockClient
)

add_library(F2DockClient STATIC
  ${SOURCE_FILES}
  ${INCLUDE_FILES}
  ${UI_FORMS}
)

target_link_libraries(F2DockClient
  Qt6::Widgets
  Qt6::Xml
  cvc::xmlrpc
)

# NOTE: SubmitDockingJob, CheckStatus, and GetResults executables removed —
# they depend on XmlRPC which was deleted in Phase 1.
# These will be replaced by cvc serve (Phase 14).
