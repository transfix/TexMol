# ===========================================================================
# Compatibility wrappers for legacy Setup*() macros
# ===========================================================================
# These macros bridge the old TexMol CMake patterns (SetupBoost, SetupQt, etc.)
# to modern find_package + target_link_libraries. They exist so that per-module
# CMakeLists.txt files can be migrated incrementally.
#
# Each macro is a no-op or thin wrapper — the actual find_package() calls are
# in the top-level CMakeLists.txt.
# ===========================================================================

# SetupQt() — Qt6 is found at top level; just link targets
macro(SetupQt)
  # No-op: Qt6 found at top level; targets use target_link_libraries(... Qt6::Widgets) etc.
endmacro()

# SetupBoost(target) — Boost comes transitively via libcvc
macro(SetupBoost target)
  # Boost is available transitively via libcvc.
  # Legacy modules that directly use Boost headers still need the include dirs.
  if(TARGET Boost::boost)
    target_link_libraries(${target} Boost::boost)
  elseif(Boost_FOUND)
    target_include_directories(${target} PRIVATE ${Boost_INCLUDE_DIRS})
  endif()
endmacro()

# SetupGSL(target) — GSL found at top level
macro(SetupGSL target)
  if(TARGET GSL::gsl)
    target_link_libraries(${target} GSL::gsl GSL::gslcblas)
  elseif(GSL_FOUND)
    target_include_directories(${target} PRIVATE ${GSL_INCLUDE_DIRS})
    target_link_libraries(${target} ${GSL_LIBRARIES})
  endif()
endmacro()

# SetupFFTW(target) — FFTW found at top level
macro(SetupFFTW target)
  if(TARGET PkgConfig::FFTW3)
    target_link_libraries(${target} PkgConfig::FFTW3)
  elseif(FFTW_FOUND)
    target_include_directories(${target} PRIVATE ${FFTW_INCLUDE_DIRS})
    target_link_libraries(${target} ${FFTW_LIBRARIES})
  endif()
endmacro()

# SetupCGAL(target) — CGAL found at top level
macro(SetupCGAL target)
  if(TARGET CGAL::CGAL)
    target_link_libraries(${target} CGAL::CGAL)
  elseif(CGAL_FOUND)
    target_include_directories(${target} PRIVATE ${CGAL_INCLUDE_DIRS})
    target_link_libraries(${target} ${CGAL_LIBRARIES})
  endif()
endmacro()

# SetupLapack(target) — LAPACK found at top level
macro(SetupLapack target)
  if(LAPACK_FOUND)
    target_link_libraries(${target} ${LAPACK_LIBRARIES})
  endif()
endmacro()

# SetupNFFT(target) — NFFT optionally found at top level
macro(SetupNFFT target)
  if(NFFT_LIBRARY)
    target_link_libraries(${target} ${NFFT_LIBRARY})
  endif()
endmacro()

# SetupPETSC(target) — PETSc optionally found at top level
macro(SetupPETSC target)
  if(PETSC_FOUND)
    target_include_directories(${target} PRIVATE ${PETSC_INCLUDE_DIRS})
    target_link_libraries(${target} ${PETSC_LIBRARIES})
  endif()
endmacro()

# SetupGLUT() — no longer used (removed)
macro(SetupGLUT)
endmacro()
