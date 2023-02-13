project(XSharp)
cmake_minimum_required(VERSION 3.5)

option(BUILD_TEST "Set to complie the test sample" ON)
option(BUILD_SHARED_LIBRARY "Set to switch to build shared library" OFF)
option(BUILD_STATIC_LIBRARY "Set to switch to build static library" ON)
option(SUPPORT_LLVMIR "Set to complie XSharpIR generator" ON)
option(SUPPORT_XSharpIR "Set to complie XSharpIR generator" ON)

include_directories(./)
file(GLOB_RECURSE SRC "XSharp/*.cpp" "XSharp/*.h")

if (SUPPORT_LLVMIR)
    aux_source_directory("LLVMIR" SRC)
      #$(llvm-config --cxxflags --ldflags --system-libs --libs core)


endif()

if (SUPPORT_XSharpIR)
    aux_source_directory("XSharpIR" SRC)
endif()

if (BUILD_SHARED_LIBRARY)
    add_definitions(-DXSharp_SHARED_LIBRARY)
    message(STATUS "Build shared library")

    add_library(XSharp_SHARED SHARED ${SRC} )
    SET_TARGET_PROPERTIES(XSharp_SHARED PROPERTIES OUTPUT_NAME "XSharp")
    target_link_libraries(XSharp_SHARED ${llvm_libs})
endif()

if(BUILD_STATIC_LIBRARY)
    add_definitions(-DXSharp_STATIC_LIBRARY)
    message(STATUS "Build static library")

    add_library(XSharp_STATIC STATIC ${SRC})
    SET_TARGET_PROPERTIES(XSharp_STATIC PROPERTIES OUTPUT_NAME "XSharp")
    target_link_libraries(XSharp_STATIC ${llvm_libs})
endif()

if (BUILD_TEST)
    add_definitions(-DXSharp_TEST)
    add_definitions(-DXSharp_TEST_UTILS)
    message(STATUS "Build test samples")

    add_executable(XSharp_TEST test/test.cpp ${SRC})
    SET_TARGET_PROPERTIES(XSharp_TEST PROPERTIES OUTPUT_NAME "XSharp_TEST")

    add_executable(XSharp_TEST_UTILS test/test_utils.cpp ${SRC})
    SET_TARGET_PROPERTIES(XSharp_TEST_UTILS PROPERTIES OUTPUT_NAME "XSharp_TEST_UTILS")

    add_executable(XSharp_TEST_LLVM test/test_llvm.cpp ${SRC})
    SET_TARGET_PROPERTIES(XSharp_TEST_LLVM PROPERTIES OUTPUT_NAME "XSharp_TEST_LLVM")

endif()

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release )
endif()

set( CMAKE_BUILD_TYPE Debug )

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++20 -fsanitize=address -Wall -Wno-unused-variable -pthread -O0 -g -ggdb -I/usr/lib/llvm-14/include -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/lib/llvm-14/lib -lLLVM-14")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++20 -O2 -pthread -fopenmp")
endif()

set(CMAKE_CXX_STANDARD 17)
