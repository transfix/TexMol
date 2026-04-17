# ===========================================================================
# CVC — Compatibility shim (Phase 8)
# The old CVC App/State/HDF5_Utilities sources are no longer compiled.
# Instead, the CVC headers in inc/CVC/ forward to libcvc (cvc::app, etc.).
# This target just exposes the compatibility headers and links to libcvc.
# ===========================================================================
cmake_minimum_required(VERSION 3.16)

add_library(CVC INTERFACE)

target_include_directories(CVC INTERFACE
  "${CMAKE_SOURCE_DIR}/inc"
)

# Link libcvc — provides cvc::app, cvc::state, etc.
target_link_libraries(CVC INTERFACE cvc)

# Link log4cplus (still used by TexMol)
target_link_libraries(CVC INTERFACE log4cplus)
