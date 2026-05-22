# ===========================================================================
# TexMol (main executable) — Qt6/AUTOMOC modernized (Phase 3)
# ===========================================================================
cmake_minimum_required(VERSION 3.16)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

if(HAVE_NFFT)
  add_definitions(-DWITH_CG -DWITH_NFFT)
endif()

if(HAVE_CGAL)
  add_definitions(-DWITH_CGAL)
endif()

if(HAVE_PETSC)
  add_definitions(-DWITH_PETSC)
  include_directories(../../inc/KIFMM3D)
  include_directories(../../inc/KIFMM3D/common)
  include_directories(../../inc/KIFMM3D/fmm3d)
  include_directories($ENV{PETSC_DIR}/$ENV{PETSC_ARCH}/include)
  include_directories($ENV{PETSC_DIR}/include)
  include_directories($ENV{PETSC_DIR}/include/mpiuni)
  link_directories($ENV{PETSC_DIR}/$ENV{PETSC_ARCH}/lib)
endif()

# HDF5
find_package(HDF5 COMPONENTS C CXX QUIET)

file(GLOB SOURCE_FILES
  ./*.cpp
  ./Dialogs/*.cpp
)
file(GLOB INCLUDE_FILES
  ../../inc/TexMol/*.[Hh]
  ../../inc/TexMol/Dialogs/*.[Hh]
)

# Exclude Qt3 source and header files
file(GLOB QT3_SOURCES ./Dialogs/*.Qt3.cpp ./*.Qt3.cpp)
file(GLOB QT3_HEADERS ../../inc/TexMol/*.Qt3.h ../../inc/TexMol/Dialogs/*.Qt3.h ../../inc/TexMol/*.Qt3.H ../../inc/TexMol/Dialogs/*.Qt3.H)
if(QT3_SOURCES)
  list(REMOVE_ITEM SOURCE_FILES ${QT3_SOURCES})
endif()
if(QT3_HEADERS)
  list(REMOVE_ITEM INCLUDE_FILES ${QT3_HEADERS})
  set_source_files_properties(${QT3_HEADERS} PROPERTIES SKIP_AUTOMOC ON SKIP_AUTOUIC ON HEADER_FILE_ONLY ON)
endif()

# Exclude dialog_stubs.cpp (no longer needed — real dialogs are ported)
list(FILTER SOURCE_FILES EXCLUDE REGEX "/dialog_stubs\\.cpp$")

# .ui files (renamed from .Qt4.ui in Phase 3)
file(GLOB UI_FORMS ./Dialogs/*.ui)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${INCLUDE_FILES})
source_group("UI Files" FILES ${UI_FORMS})

include_directories(
  ../../inc
  ${CMAKE_CURRENT_BINARY_DIR}     # for uic-generated headers
  ../../inc/ColorTable2            # for constructDepthColoredVolumesDialogBase.ui
  ${CMAKE_BINARY_DIR}/src/F2DockClient
)

set(MACOSX_BUNDLE_ICON_FILE ${TexMol_SOURCE_DIR}/dat/TexMol.icns)

add_executable(TexMol MACOSX_BUNDLE
  ${SOURCE_FILES}
  ${INCLUDE_FILES}
  ${UI_FORMS}
)

# AUTOUIC needs to find .ui files for headers in inc/ that reference ui_ headers
set_target_properties(TexMol PROPERTIES
  AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/Dialogs"
)

# ---------------------------------------------------------------------------
# Link libraries
# ---------------------------------------------------------------------------
# Phase 1 deleted modules replaced by libcvc (cvc target):
#   SimpleVolumeData, VolMagick, VolumeFileTypes, VolumeLibrary,
#   SignDistanceFunction, SignDistanceFunction_v2, Contouring, Contour,
#   LBIE_lib, Geometry, ComputeNormals, ByteOrder, CVC (App/State),
#   Utility, XmlRPC, GeometryFileTypes, GraphicsPrimitives
set(TEXMOL_LINK_LIBS
  # libcvc — replaces all Phase 1 deleted modules
  cvc::cvc

  # Remaining TexMol modules
  Decimation
  ASpline
  OptimizerLib
  F2DockClient
  DataManager
  Blurmaps
  C2C
  arithlib
  libdjvu
  ColorSurfaceByFunction
  Curvatures
  GeometryRenderer
  HLevelSet
  ARM
  Optimizer
  MolecularCharacteristics
  Pocket
  SmoothingFunction
  SphericalPatchIntersections
  Summation
  UsefulMath
  Nurbs
  vis-lib
  fast-hydro
  fast-resCont
  DPG
  OpenGL_Viewer
  ImposterRenderer
  SplattingRenderer
  Histogram
  ObjectRenderer
  RayTracer
  PatchComplex
  GOAFileTypes
  PDBParser
  ColorTable2
  ColorManager
  MolEnergyUtil

  # Internal compatibility libraries (restored from removed modules)
  Geometry
  GeometryFileTypes
  SimpleVolumeData
  Utility
  ComputeNormals
  LBIE_lib
  SignDistanceFunction_v2
  VolumeFileTypes
  VolumeLibrary
  Contouring
  Contour
  cvc::xmlrpc
  ARM
  OptimizerLib
  CoarseGrain
  CVC
  contourtree
  levmar

  # System
  Qt6::Widgets
  Qt6::Gui
  Qt6::Core
  Qt6::Xml
  Qt6::Network
  Qt6::OpenGL
  Qt6::OpenGLWidgets
  OpenGL::GL
  OpenGL::GLU
  Threads::Threads
)

if(ENABLE_MULTIRESOL_SURFACE)
  list(APPEND TEXMOL_LINK_LIBS MultiresolutionSurface)
endif()

if(LAPACK_FOUND)
  list(APPEND TEXMOL_LINK_LIBS ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES})
endif()

if(ENABLE_POCKET_TUNNEL)
  list(APPEND TEXMOL_LINK_LIBS PocketTunnel)
endif()

if(ENABLE_SECONDARY_STRUCTURE)
  list(APPEND TEXMOL_LINK_LIBS SecondaryStructures)
endif()

if(BUILD_INTERFACESTATS)
  list(APPEND TEXMOL_LINK_LIBS InterfaceStats)
endif()

if(BUILD_MOLSURF_API)
  list(PREPEND TEXMOL_LINK_LIBS MolSurfAPI)
endif()

if(HAVE_NFFT)
  list(APPEND TEXMOL_LINK_LIBS CoarseGrain GBEnergy levmar FastSummation ${NFFT_LIBRARY} fftw3)
endif()

if(HAVE_PETSC)
  list(APPEND TEXMOL_LINK_LIBS PB2 TriaElement KIFMM3D KIFMM3DCommon petsc mpich)
  if(APPLE)
    list(APPEND TEXMOL_LINK_LIBS f2cblas f2clapack pmpich ${X11_LIBRARIES})
  endif()
endif()

if(HAVE_FFTW)
  list(APPEND TEXMOL_LINK_LIBS ${FTWD_LIB} ${FFTWD_THREADS_LIB})
endif()

if(HDF5_FOUND)
  list(APPEND TEXMOL_LINK_LIBS ${HDF5_LIBRARIES})
endif()

# GLEW — system (required)
list(APPEND TEXMOL_LINK_LIBS GLEW::GLEW)

# Legacy GL for extension function pointers (glBindFramebufferEXT etc.)
list(APPEND TEXMOL_LINK_LIBS GL)

target_link_libraries(TexMol ${TEXMOL_LINK_LIBS})

SetupBoost(TexMol)
SetupCGAL(TexMol)

# for secondary structure includes
add_definitions(-DHUGE=1000000)

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
install(TARGETS TexMol
  RUNTIME DESTINATION bin
  BUNDLE DESTINATION .
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

if(UNIX AND NOT APPLE)
  install(PROGRAMS "${TexMol_SOURCE_DIR}/bin/runTexMol" DESTINATION bin OPTIONAL)
endif()

