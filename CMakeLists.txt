project(XSharp)
cmake_minimum_required(VERSION 3.14)

option(BUILD_TEST "Set to complie the test sample" ON)
option(BUILD_SHARED_LIBRARY "Set to switch to build shared library" OFF)
option(BUILD_STATIC_LIBRARY "Set to switch to build static library" ON)
option(BUILD_XSharpCLI "Set to complie XSharp's command-line interface tool" ON)
option(BUILD_XSharpRuntime "Set to complie XSharp's runtime for language" ON)
option(SUPPORT_LLVMIR "Set to complie XSharpIR generator" ON)
option(SUPPORT_XSharpIR "Set to complie XSharpIR generator" ON)

include_directories(./)
file(GLOB_RECURSE SRC "XSharp/*.cpp" "XSharp/*.h")
file(GLOB_RECURSE RUNTIME_SRC "XSharpRuntime/*.cpp" "XSharpRuntime/*.c"
     "XSharpRuntime/*.h")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

make_directory(${PROJECT_SOURCE_DIR}/bin)
make_directory(${PROJECT_SOURCE_DIR}/test/bin)
make_directory(${PROJECT_SOURCE_DIR}/lib)

if(SUPPORT_LLVMIR)
  aux_source_directory("LLVMIR" SRC)
  add_definitions(-DXSharp_LLVMIR_SUPPORT)

  find_package(LLVM REQUIRED CONFIG)

  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

  include_directories(${LLVM_INCLUDE_DIRS})
  separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
  add_definitions(${LLVM_DEFINITIONS_LIST})

  llvm_map_components_to_libnames(llvm_libs all support core codegen irreader)

  message(STATUS "LLVM LIBS: ${llvm_libs}")

endif()

# Third Parties
include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9.1.0)

FetchContent_MakeAvailable(fmt)

if(SUPPORT_XSharpIR)
  aux_source_directory("XSharpIR" SRC)
  add_definitions(-DXSharp_XSharpIR_SUPPORT)
endif()

if(BUILD_SHARED_LIBRARY)
  add_definitions(-DXSharp_SHARED_LIBRARY)
  message(STATUS "Build shared library")

  add_library(XSharp_SHARED SHARED ${SRC})
  set_target_properties(XSharp_SHARED PROPERTIES OUTPUT_NAME "XSharp")
  set_target_properties(XSharp_SHARED PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                                                 "${PROJECT_SOURCE_DIR}/lib")

endif()

if(BUILD_STATIC_LIBRARY)
  add_definitions(-DXSharp_STATIC_LIBRARY)
  message(STATUS "Build static library")

  add_library(XSharp_STATIC STATIC ${SRC})
  set_target_properties(XSharp_STATIC PROPERTIES OUTPUT_NAME "XSharp")
  set_target_properties(XSharp_STATIC PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                                                 "${PROJECT_SOURCE_DIR}/lib")

  if(SUPPORT_LLVMIR)
    target_link_libraries(XSharp_STATIC fmt ${llvm_libs})
  endif()
endif()

if(BUILD_TEST)
  add_subdirectory(test)
  enable_testing()
  add_test(NAME test1 COMMAND $<TARGET_FILE:TEST_UTILS>)
  add_test(NAME test2 COMMAND $<TARGET_FILE:TEST_LLVM>)
  add_test(NAME test3 COMMAND $<TARGET_FILE:TEST_ALL>)
  # message(STATUS "Build test samples")

  # make_directory(test/bin) add_executable(XSharp_TEST test/test.cpp)
  # set_target_properties( XSharp_TEST PROPERTIES OUTPUT_NAME
  # "${CMAKE_SOURCE_DIR}/test/bin/XSharp_TEST")

  # add_executable(XSharp_TEST_UTILS test/test_utils.cpp) set_target_properties(
  # XSharp_TEST_UTILS PROPERTIES OUTPUT_NAME
  # "${CMAKE_SOURCE_DIR}/test/bin/XSharp_TEST_UTILS")

  # add_executable(XSharp_TEST_LLVM test/test_llvm.cpp) set_target_properties(
  # XSharp_TEST_LLVM PROPERTIES OUTPUT_NAME
  # "${CMAKE_SOURCE_DIR}/test/bin/XSharp_TEST_LLVM")

  # target_link_libraries(XSharp_TEST XSharp_STATIC)
  # target_link_libraries(XSharp_TEST_UTILS XSharp_STATIC)
  # target_link_libraries(XSharp_TEST_LLVM XSharp_STATIC)
endif()

# Shared Library is necessary
if(BUILD_XSharpRuntime)
  add_subdirectory(XSharpRuntime)
endif()

if(BUILD_XSharpCLI)
  message(STATUS "Build XSharp's CLI")

  add_executable(XSharpC XSharpCLI/xsharpc.cpp)
  set_target_properties(XSharpC PROPERTIES OUTPUT_NAME
                                           "${CMAKE_SOURCE_DIR}/bin/xsharpc")
  if(SUPPORT_LLVMIR)
    target_link_libraries(XSharpC ${llvm_libs} XSharp_STATIC)
  endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address  -Wall -Wno-unused-variable -pthread -O0 -g -ggdb -std=c++20"
  )
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} -std=c++20  -O2 -pthread -fopenmp")
endif()

set(CMAKE_CXX_STANDARD 20)
