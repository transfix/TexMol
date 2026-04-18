# ===========================================================================
# Unit tests
# ===========================================================================

# --- UsefulMath (pure math, no external deps) ---
texmol_add_test(test_vector test_vector.cpp)
target_link_libraries(test_vector PRIVATE UsefulMath)

texmol_add_test(test_quaternion test_quaternion.cpp)
target_link_libraries(test_quaternion PRIVATE UsefulMath)

texmol_add_test(test_matrix test_matrix.cpp)
target_link_libraries(test_matrix PRIVATE UsefulMath)

# --- IntQueue (self-contained data structure, compiled standalone to avoid GL deps) ---
texmol_add_test(test_geometry test_geometry.cpp
  ${CMAKE_SOURCE_DIR}/src/Geometry/IntQueue.cpp)
