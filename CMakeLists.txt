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
# Default: use prebuilt libcvc binary from CMAKE_PREFIX_PATH
#   (binary release from https://github.com/transfix/libcvc/releases)
# Co-dev: cmake -B build -DLIBCVC_SOURCE_DIR=/path/to/libcvc
# ---------------------------------------------------------------------------
option(LIBCVC_SOURCE_DIR "Path to local libcvc source tree (empty = use prebuilt binary)" "")

if(LIBCVC_SOURCE_DIR)
  message(STATUS "Using local libcvc: ${LIBCVC_SOURCE_DIR}")
  list(PREPEND CMAKE_MODULE_PATH "${LIBCVC_SOURCE_DIR}/CMake")
  set(CVC_BUILD_VOLROVER3 OFF CACHE BOOL "" FORCE)
  add_subdirectory("${LIBCVC_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/libcvc")
else()
  # ── Prebuilt libcvc binary ──
  # The official cvcConfig.cmake export encodes build-time deps
  # (CUDA, IMOD MRC, hardcoded system paths) that a consumer may
  # not have.  Create thin imported targets directly instead.
  find_path(LIBCVC_INCLUDE_DIR cvc/app.h PATH_SUFFIXES include)
  find_library(LIBCVC_LIBRARY NAMES cvc)
  find_library(LIBCVC_XMLRPC_LIBRARY NAMES xmlrpc)

  if(NOT LIBCVC_INCLUDE_DIR OR NOT LIBCVC_LIBRARY)
    message(FATAL_ERROR
      "Prebuilt libcvc not found.\n"
      "Set CMAKE_PREFIX_PATH to the libcvc install prefix,\n"
      "or set LIBCVC_SOURCE_DIR for co-development builds.")
  endif()

  add_library(cvc::cvc SHARED IMPORTED GLOBAL)
  set_target_properties(cvc::cvc PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${LIBCVC_INCLUDE_DIR}"
  )
  if(WIN32)
    find_file(LIBCVC_DLL NAMES cvc.dll PATH_SUFFIXES bin)
    set_target_properties(cvc::cvc PROPERTIES
      IMPORTED_IMPLIB "${LIBCVC_LIBRARY}"
      IMPORTED_LOCATION "${LIBCVC_DLL}"
    )
  else()
    set_target_properties(cvc::cvc PROPERTIES
      IMPORTED_LOCATION "${LIBCVC_LIBRARY}"
    )
  endif()

  if(LIBCVC_XMLRPC_LIBRARY)
    add_library(cvc::xmlrpc STATIC IMPORTED GLOBAL)
    set_target_properties(cvc::xmlrpc PROPERTIES
      IMPORTED_LOCATION "${LIBCVC_XMLRPC_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${LIBCVC_INCLUDE_DIR}"
    )
    if(WIN32)
      target_link_libraries(cvc::xmlrpc INTERFACE wsock32 ws2_32)
    endif()
  endif()

  message(STATUS "Using prebuilt libcvc: ${LIBCVC_LIBRARY}")
endif()

# Legacy compatibility macros for per-module CMakeLists.txt files
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
  include_directories(BEFORE "${LIBCVC_INCLUDE_DIR}")
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

# ---------------------------------------------------------------------------
# Windows / MSVC portability
# ---------------------------------------------------------------------------
if(MSVC)
  # Avoid C++17 std::byte vs Windows SDK byte ambiguity
  add_compile_definitions(_HAS_STD_BYTE=0)
  # Suppress CRT deprecation warnings for POSIX names
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  # Map POSIX string functions to MSVC equivalents
  add_compile_definitions(strcasecmp=_stricmp strncasecmp=_strnicmp)
  # M_PI and friends are not defined by default on MSVC
  add_compile_definitions(_USE_MATH_DEFINES)
  # Map POSIX random functions to CRT equivalents
  add_compile_definitions(srand48\(x\)=srand\(\(unsigned\)\(x\)\))
  add_compile_definitions(drand48\(\)=\(rand\(\)/\(double\)RAND_MAX\))

  # PThreads4W — provides <pthread.h> on Windows (vcpkg)
  find_package(PThreads4W QUIET)
  if(PThreads4W_FOUND)
    # Expose pthread.h include path globally so legacy #include <pthread.h> works
    get_target_property(_ptw4_inc PThreads4W::PThreads4W INTERFACE_INCLUDE_DIRECTORIES)
    if(_ptw4_inc)
      include_directories(${_ptw4_inc})
    endif()
  endif()

  # Compat stubs for POSIX headers (unistd.h, sys/time.h)
  include_directories("${CMAKE_SOURCE_DIR}/inc/compat")

  # Map POSIX sleep(sec) to Windows Sleep(ms)
  add_compile_definitions(sleep\(x\)=Sleep\(1000*\(x\)\))
endif()

# GLEW (required — replaces bundled glew and hand-rolled StaticExtensionPointers)
find_package(GLEW REQUIRED)

# Boost — used directly by several legacy modules (ColorTable2, etc.) via
# SetupBoost(target). libcvc consumes Boost privately, so we need our own
# top-level find_package to expose Boost::boost as an imported target.
find_package(Boost REQUIRED)

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
