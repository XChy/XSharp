message(STATUS "Build XSharp's Runtime")

include(FetchContent)

FetchContent_Declare(
  gc
  GIT_REPOSITORY https://github.com/ivmai/bdwgc
  GIT_TAG v8.2.2)

FetchContent_MakeAvailable(gc)

add_library(XSharp_RUNTIME_DYNAMIC SHARED ${RUNTIME_SRC})
target_link_libraries(XSharp_RUNTIME_DYNAMIC gc)
set_target_properties(XSharp_RUNTIME_DYNAMIC PROPERTIES OUTPUT_NAME
                                                        "XSharpRuntime")
set_target_properties(
  XSharp_RUNTIME_DYNAMIC PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                                    "${PROJECT_SOURCE_DIR}/lib")

set(CMAKE_CXX_FLAGS "-std=c++20 -O2")
