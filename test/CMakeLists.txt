message(STATUS "Build XSharp's Test")

include_directories(.)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.0)
FetchContent_MakeAvailable(googletest)

add_executable(TEST_LLVM test_llvm.cpp)
set_target_properties(TEST_LLVM PROPERTIES OUTPUT_NAME "test_llvm")

add_executable(TEST_UTILS test_utils.cpp)
set_target_properties(TEST_UTILS PROPERTIES OUTPUT_NAME "test_utils")

add_executable(TEST_ALL test.cpp)
set_target_properties(TEST_ALL PROPERTIES OUTPUT_NAME "test")

target_link_libraries(TEST_LLVM gtest_main XSharp_STATIC)
target_link_libraries(TEST_UTILS gtest_main XSharp_STATIC)
target_link_libraries(TEST_ALL gtest_main XSharp_STATIC)

set(CMAKE_CXX_FLAGS "-std=c++20 -O2  -fsanitize=address ")
