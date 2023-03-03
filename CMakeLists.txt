project(XSharp)
cmake_minimum_required(VERSION 3.5)

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

if(SUPPORT_LLVMIR)
  aux_source_directory("LLVMIR" SRC)
  add_definitions(-DXSharp_LLVMIR_SUPPORT)
endif()

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
                                                 "shared")
  # target_link_libraries(XSharp_SHARED ${llvm_libs})

endif()

if(BUILD_STATIC_LIBRARY)
  add_definitions(-DXSharp_STATIC_LIBRARY)
  message(STATUS "Build static library")

  add_library(XSharp_STATIC STATIC ${SRC})
  set_target_properties(XSharp_STATIC PROPERTIES OUTPUT_NAME "XSharp")
  set_target_properties(XSharp_STATIC PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib")

  # target_link_libraries(XSharp_STATIC ${llvm_libs})
endif()

if(BUILD_TEST)
  message(STATUS "Build test samples")

  make_directory(test/bin)
  add_executable(XSharp_TEST test/test.cpp ${SRC})
  set_target_properties(
    XSharp_TEST PROPERTIES OUTPUT_NAME
                           "${CMAKE_SOURCE_DIR}/test/bin/XSharp_TEST")

  add_executable(XSharp_TEST_UTILS test/test_utils.cpp ${SRC})
  set_target_properties(
    XSharp_TEST_UTILS
    PROPERTIES OUTPUT_NAME "${CMAKE_SOURCE_DIR}/test/bin/XSharp_TEST_UTILS")

  add_executable(XSharp_TEST_LLVM test/test_llvm.cpp ${SRC})
  set_target_properties(
    XSharp_TEST_LLVM PROPERTIES OUTPUT_NAME
                                "${CMAKE_SOURCE_DIR}/test/bin/XSharp_TEST_LLVM")
endif()

# Shared Library is necessary
if(BUILD_XSharpRuntime)
  add_subdirectory(XSharpRuntime)
endif()

if(BUILD_XSharpCLI)
  message(STATUS "Build XSharp's CLI")

  add_executable(XSharpC XSharpCLI/xsharpc.cpp ${SRC})
  set_target_properties(XSharpC PROPERTIES OUTPUT_NAME
                                           "${CMAKE_SOURCE_DIR}/bin/xsharpc")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# TODO: compliment cmake's setting
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -std=c++20 -fsanitize=address -Wall -Wno-unused-variable -pthread -O0 -g -ggdb -I/usr/lib/llvm-14/include -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/lib/llvm-14/lib -lLLVM-14"
  )
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} -std=c++20 -O2 -pthread -fopenmp")
endif()

set(CMAKE_CXX_STANDARD 20)
