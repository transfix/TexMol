# ===========================================================================
# TexMol — Molecular Processing and Visualization
# ===========================================================================
# Modernized CMake build (Phase 2+3 of CVC Modernization Plan)
# - CMake 3.16+ with target-based patterns
# - libcvc as core dependency (FetchContent or local)
# - No third-party/ ExternalProject chain
# - GLSL only (Cg removed)
# ===========================================================================
cmake_minimum_required(VERSION 3.16)

project(TexMol
  VERSION 2.1.0
  DESCRIPTION "Molecular Processing and Visualization"
  LANGUAGES C CXX
)

# ---------------------------------------------------------------------------
# C++ Standard
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Suppress Boost bind global placeholders deprecation warning
add_compile_definitions(BOOST_BIND_GLOBAL_PLACEHOLDERS)

# ---------------------------------------------------------------------------
# Testing and coverage options
# ---------------------------------------------------------------------------
option(TEXMOL_BUILD_TESTS "Build unit tests" OFF)
option(TEXMOL_ENABLE_COVERAGE "Enable code coverage instrumentation (GCC/Clang)" OFF)

# ---------------------------------------------------------------------------
# Output directories
# ---------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Qt6 auto-processing (replaces manual QT_WRAP_CPP / QT4_WRAP_UI calls)
# ---------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---------------------------------------------------------------------------
# CMake modules
# ---------------------------------------------------------------------------
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

# ---------------------------------------------------------------------------
# GLSL rendering (Cg removed — discontinued by NVIDIA)
# ---------------------------------------------------------------------------
add_compile_definitions(GLSL)

# ---------------------------------------------------------------------------
# libcvc — Core dependency
# ---------------------------------------------------------------------------
# Option A (default): FetchContent from GitHub
# Option B: Set LIBCVC_SOURCE_DIR to a local checkout for co-development
#   cmake -B build -DLIBCVC_SOURCE_DIR=/path/to/libcvc
# ---------------------------------------------------------------------------
option(LIBCVC_SOURCE_DIR "Path to local libcvc source tree (empty = use FetchContent)" "")

if(LIBCVC_SOURCE_DIR)
  message(STATUS "Using local libcvc: ${LIBCVC_SOURCE_DIR}")
  # Prepend libcvc's CMake module path so its Setup*.cmake modules are found
  # (libcvc uses CMAKE_SOURCE_DIR which points to TexMol when used as subdirectory)
  list(PREPEND CMAKE_MODULE_PATH "${LIBCVC_SOURCE_DIR}/CMake")
  # Disable volrover3 — it uses CMAKE_SOURCE_DIR for source paths
  set(CVC_BUILD_VOLROVER3 OFF CACHE BOOL "" FORCE)
  add_subdirectory("${LIBCVC_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/libcvc")
else()
  include(FetchContent)
  FetchContent_Declare(libcvc
    GIT_REPOSITORY https://github.com/transfix/libcvc.git
    GIT_TAG        95ff1bb8c0053179aa6c8b66ac793be5a6fafb5f # v3.1.0
  )
  # Configure libcvc build options
  set(CVC_ENABLE_CUDA OFF CACHE BOOL "Disable CUDA in TexMol build by default")
  set(CVC_USING_HDF5 ON CACHE BOOL "Enable HDF5 volume I/O")
  set(CVC_ENABLE_MESHER ON CACHE BOOL "Enable isosurface + volumetric meshing")
  set(CVC_ENABLE_SDF ON CACHE BOOL "Enable SDF computation")
  # XmlRPC is provided by libcvc as cvc::xmlrpc (consumed by F2DockClient)
  set(CVC_USING_XMLRPC ON CACHE BOOL "Build libcvc's vendored xmlrpc library" FORCE)
  FetchContent_MakeAvailable(libcvc)
endif()

# Legacy compatibility macros for per-module CMakeLists.txt files
# MUST be included AFTER libcvc so our plain-signature wrappers override
# libcvc's keyword-signature SetupBoost/SetupCGAL functions.
include(LegacyCompat)

# ---------------------------------------------------------------------------
# Make libcvc headers available globally (for compatibility shims in inc/)
# Use BEFORE so that <cvc/foo.h> always resolves to libcvc's copy, not to
# TexMol's legacy case-insensitive <CVC/Foo.h> compat headers.
# ---------------------------------------------------------------------------
if(LIBCVC_SOURCE_DIR)
  include_directories(BEFORE "${LIBCVC_SOURCE_DIR}/inc")
  include_directories(BEFORE "${CMAKE_BINARY_DIR}/libcvc/inc")  # generated cvc/config.h
else()
  include_directories(BEFORE "${libcvc_SOURCE_DIR}/inc")
  include_directories(BEFORE "${libcvc_BINARY_DIR}/inc")
endif()

# ---------------------------------------------------------------------------
# System dependencies
# ---------------------------------------------------------------------------
# Qt6
find_package(Qt6 COMPONENTS Core Gui Widgets Xml Network OpenGL OpenGLWidgets QUIET)
if(NOT Qt6_FOUND)
  message(FATAL_ERROR "Qt6 not found. Install: sudo apt-get install qt6-base-dev libqt6opengl6-dev libqt6openglwidgets6")
endif()

# OpenGL
find_package(OpenGL REQUIRED)

# Threads (POSIX pthreads on Linux/macOS, vcpkg PThreads4W on Windows)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# GLEW (required — replaces bundled glew and hand-rolled StaticExtensionPointers)
find_package(GLEW REQUIRED)

# GSL (for molecular analysis modules)
find_package(GSL QUIET)

# FFTW (for FastSummation, GBEnergy, MultiresolutionSurface)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(FFTW3 IMPORTED_TARGET fftw3)
endif()
if(NOT FFTW3_FOUND)
  # Try CMake module path
  find_package(FFTW QUIET)
endif()
set(HAVE_FFTW ${FFTW3_FOUND} CACHE BOOL "FFTW3 available")

# ---------------------------------------------------------------------------
# Optional dependencies
# ---------------------------------------------------------------------------
option(HAVE_CGAL "Use CGAL-dependent functions (SecondaryStructures, PocketTunnel)" ON)
option(HAVE_NFFT "Use NFFT-dependent functions (FastSummation, GBEnergy)" OFF)
option(HAVE_PETSC "Use PETSc-dependent functions (KIFMM3D, PB2)" OFF)
option(HAVE_LAPACK "Use LAPACK-dependent functions" ON)

option(ENABLE_SECONDARY_STRUCTURE "Enable secondary structure analysis" ON)
option(ENABLE_POCKET_TUNNEL "Enable pocket-tunnel detection" ON)
option(BUILD_INTERFACESTATS "Enable InterfaceStats module" ON)
option(BUILD_MOLSURF_API "Enable MolSurfAPI module" ON)
option(ENABLE_MULTIRESOL_SURFACE "Enable multiresolution surface" ON)
option(TEXMOL_ENABLE_ROUNDING_MATH "Enable -frounding-math (needed for CGAL on some compilers)" OFF)

if(HAVE_CGAL)
  find_package(CGAL QUIET)
  if(NOT CGAL_FOUND)
    message(WARNING "CGAL not found — disabling CGAL-dependent modules")
    set(HAVE_CGAL OFF)
  endif()
endif()

if(HAVE_LAPACK)
  find_package(LAPACK QUIET)
  if(NOT LAPACK_FOUND)
    message(WARNING "LAPACK not found — disabling LAPACK-dependent modules")
    set(HAVE_LAPACK OFF)
  endif()
endif()

if(HAVE_NFFT)
  # NFFT is rare; make it genuinely optional
  find_library(NFFT_LIBRARY nfft3)
  if(NOT NFFT_LIBRARY)
    message(WARNING "NFFT not found — disabling NFFT-dependent modules")
    set(HAVE_NFFT OFF)
  endif()
endif()

if(HAVE_PETSC)
  find_package(PETSc QUIET)
  if(NOT PETSC_FOUND)
    message(WARNING "PETSc not found — disabling PETSc-dependent modules")
    set(HAVE_PETSC OFF)
  endif()
endif()

# ---------------------------------------------------------------------------
# Global compile definitions for feature flags
# ---------------------------------------------------------------------------
if(ENABLE_SECONDARY_STRUCTURE AND HAVE_CGAL AND HAVE_LAPACK)
  add_compile_definitions(SS)
else()
  set(ENABLE_SECONDARY_STRUCTURE OFF)
endif()

if(ENABLE_POCKET_TUNNEL AND HAVE_CGAL AND HAVE_LAPACK)
  add_compile_definitions(POCKET_TUNNEL)
else()
  set(ENABLE_POCKET_TUNNEL OFF)
endif()

if(BUILD_INTERFACESTATS AND HAVE_LAPACK)
  add_compile_definitions(IFS)
else()
  set(BUILD_INTERFACESTATS OFF)
endif()

if(BUILD_MOLSURF_API AND HAVE_LAPACK)
  add_compile_definitions(MSA)
else()
  set(BUILD_MOLSURF_API OFF)
endif()

if(ENABLE_MULTIRESOL_SURFACE AND HAVE_FFTW)
  add_compile_definitions(MS)
else()
  set(ENABLE_MULTIRESOL_SURFACE OFF)
endif()

if(TEXMOL_ENABLE_ROUNDING_MATH)
  add_compile_options(-frounding-math)
endif()

# ---------------------------------------------------------------------------
# Status report
# ---------------------------------------------------------------------------
message(STATUS "=== TexMol ${PROJECT_VERSION} Build Configuration ===")
message(STATUS "  C++ Standard:           ${CMAKE_CXX_STANDARD}")
message(STATUS "  Qt6:                    ${Qt6_VERSION}")
message(STATUS "  libcvc:                 ${LIBCVC_SOURCE_DIR}")
message(STATUS "  CGAL:                   ${HAVE_CGAL}")
message(STATUS "  LAPACK:                 ${HAVE_LAPACK}")
message(STATUS "  FFTW:                   ${HAVE_FFTW}")
message(STATUS "  NFFT:                   ${HAVE_NFFT}")
message(STATUS "  PETSc:                  ${HAVE_PETSC}")
message(STATUS "  Secondary Structure:    ${ENABLE_SECONDARY_STRUCTURE}")
message(STATUS "  Pocket Tunnel:          ${ENABLE_POCKET_TUNNEL}")
message(STATUS "  InterfaceStats:         ${BUILD_INTERFACESTATS}")
message(STATUS "  MolSurfAPI:             ${BUILD_MOLSURF_API}")
message(STATUS "  Multiresolution Surf:   ${ENABLE_MULTIRESOL_SURFACE}")
message(STATUS "===============================================")

# ---------------------------------------------------------------------------
# Source tree
# ---------------------------------------------------------------------------
add_subdirectory(src)

# ---------------------------------------------------------------------------
# macOS Bundle
# ---------------------------------------------------------------------------
if(APPLE)
  set_target_properties(TexMol PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "edu.utexas.cvc.texmol"
    MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
  )
endif()

# ---------------------------------------------------------------------------
# Code coverage (must be before targets that use it)
# ---------------------------------------------------------------------------
if(TEXMOL_ENABLE_COVERAGE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Code coverage enabled")
    set(COVERAGE_COMPILE_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
    set(COVERAGE_LINK_FLAGS "--coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")

    find_program(LCOV_PATH lcov)
    find_program(GENHTML_PATH genhtml)

    if(LCOV_PATH AND GENHTML_PATH)
      add_custom_target(coverage
        COMMAND ${LCOV_PATH} --directory . --zerocounters
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info --ignore-errors mismatch
        COMMAND ${LCOV_PATH} --remove coverage.info
          '/usr/*' '*/test/*' '*/tests/*' '*/_deps/*' '*/googletest/*'
          --output-file coverage_filtered.info
        COMMAND ${GENHTML_PATH} coverage_filtered.info
          --output-directory coverage_html
          --title "TexMol Code Coverage"
          --legend --demangle-cpp --ignore-errors source
        COMMAND ${CMAKE_COMMAND} -E echo "Coverage report: ${CMAKE_BINARY_DIR}/coverage_html/index.html"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating code coverage report"
      )
    else()
      message(WARNING "lcov/genhtml not found — coverage target unavailable (apt install lcov)")
    endif()
  else()
    message(WARNING "Code coverage only supported with GCC or Clang")
  endif()
endif()

# ---------------------------------------------------------------------------
# Testing (Google Test)
# ---------------------------------------------------------------------------
if(TEXMOL_BUILD_TESTS)
  enable_testing()
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  message(STATUS "Google Test enabled — tests will be built")
  add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# CPack packaging
# ---------------------------------------------------------------------------
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Molecular Processing and Visualization")
set(CPACK_PACKAGE_VENDOR "Computational Visualization Center")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "arand@ices.utexas.edu")
set(CPACK_STRIP_FILES ON)
set(CPACK_PACKAGE_EXECUTABLES "TexMol" "TexMol")

if(WIN32)
  set(CPACK_GENERATOR "NSIS")
elseif(APPLE)
  set(CPACK_GENERATOR "DragNDrop")
else()
  set(CPACK_GENERATOR "DEB;TGZ")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "CVC Lab")
endif()
include(CPack)
