# ===========================================================================
# TexMol Test Suite
# ===========================================================================
# Build with: cmake -B build-test -DTEXMOL_BUILD_TESTS=ON
# Run with:   ctest --test-dir build-test --output-on-failure
# Coverage:   cmake -B build-test -DTEXMOL_BUILD_TESTS=ON -DTEXMOL_ENABLE_COVERAGE=ON
#             cmake --build build-test && cmake --build build-test --target coverage
# ===========================================================================

include_directories(${CMAKE_SOURCE_DIR}/inc)

# ---------------------------------------------------------------------------
# Helper: add a test executable linked to GTest and TexMol libraries
# ---------------------------------------------------------------------------
function(texmol_add_test TEST_NAME)
  add_executable(${TEST_NAME} ${ARGN})
  target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest_main)
  target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

# ---------------------------------------------------------------------------
# Unit tests
# ---------------------------------------------------------------------------
add_subdirectory(unit)
