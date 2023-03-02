message(STATUS "Build XSharp's Runtime")

add_library(XSharp_RUNTIME_DYNAMIC SHARED ${RUNTIME_SRC})
set_target_properties(XSharp_RUNTIME_DYNAMIC PROPERTIES OUTPUT_NAME
                                                        "XSharpRuntime")
set_target_properties(
  XSharp_RUNTIME_DYNAMIC PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                                    "${PROJECT_SOURCE_DIR}/lib")
# set_target_properties(XSharp_RUNTIME_DYNAMIC PROPERTIES COMPILE_FLAGS
# "-std=c++17 -O2")
set(CMAKE_CXX_FLAGS "-std=c++20 -O2")
