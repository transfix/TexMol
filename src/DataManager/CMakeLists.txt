# ===========================================================================
# DataManager — Qt6/AUTOMOC modernized (Phase 3)
# ===========================================================================
cmake_minimum_required(VERSION 3.16)

if(HAVE_CGAL)
  add_definitions(-DWITH_CGAL)
endif()

if(ENABLE_SECONDARY_STRUCTURE)
  add_definitions(-DWITH_SECSTRUCT)
endif()

# for secondary structure includes (must not collide with POSIX/UCRT HUGE macro)
add_definitions(-DHUGE_COST=1000000)

file(GLOB SOURCE_FILES
  ./*.cpp
  ./SurfaceDataManager/*.cpp
  ./BallAndStickDataManager/*.cpp
  ./SecondaryStructureDataManager/*.cpp
  ./NURBSDataManager/*.cpp
  ./VolumeDataManager/*.cpp
)

# Exclude Qt3 source files — these need Qt3Support which is not available in Qt6
# To be ported in Phase 6 (Qt6 source migration)
file(GLOB QT3_FILES ./*/*.Qt3.cpp)
list(REMOVE_ITEM SOURCE_FILES ${QT3_FILES})

# Also exclude Qt3 header files from AUTOMOC processing
file(GLOB QT3_HEADERS ../../inc/DataManager/*/*.Qt3.h)
set_source_files_properties(${QT3_HEADERS} PROPERTIES SKIP_AUTOMOC ON SKIP_AUTOUIC ON HEADER_FILE_ONLY ON)

# Exclude duplicate headers in src/ that conflict with inc/ (AUTOMOC picks up both)
file(GLOB SRC_DUP_HEADERS ./*/*.h ./*/*.H)
set_source_files_properties(${SRC_DUP_HEADERS} PROPERTIES SKIP_AUTOMOC ON)

# .ui files (renamed from .Qt4.ui in Phase 3)
file(GLOB UI_FORMS
  ./SurfaceDataManager/*.ui
  ./BallAndStickDataManager/*.ui
  ./SecondaryStructureDataManager/*.ui
  ./NURBSDataManager/*.ui
  ./VolumeDataManager/*.ui
)

file(GLOB INCLUDE_FILES
  ../../inc/DataManager/*.[Hh]
  ../../inc/DataManager/BallAndStickDataManager/*.[Hh]
  ../../inc/DataManager/SecondaryStructureDataManager/*.[Hh]
  ../../inc/DataManager/NURBSDataManager/*.[Hh]
  ../../inc/DataManager/SurfaceDataManager/*.[Hh]
  ../../inc/DataManager/VolumeDataManager/*.[Hh]
)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("UI Files" FILES ${UI_FORMS})
source_group("Header Files" FILES ${INCLUDE_FILES})

# AUTOMOC/AUTOUIC need the include dirs to find headers referenced from .ui
include_directories(
  ../../inc
  ${CMAKE_CURRENT_BINARY_DIR}   # for uic-generated headers
  ../../inc/Histogram            # for SecondaryStructureRenderingPropertiesBase.ui
  ../../inc/ColorTable           # for VolumeRenderingPropertiesBase.ui
)

add_library(DataManager STATIC
  ${SOURCE_FILES}
  ${INCLUDE_FILES}
  ${UI_FORMS}
)

target_link_libraries(DataManager
  Qt6::Widgets
  Qt6::OpenGLWidgets
)

SetupBoost(DataManager)
SetupCGAL(DataManager)
