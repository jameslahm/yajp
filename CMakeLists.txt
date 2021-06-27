cmake_minimum_required(VERSION 3.0.0)
project(yajp VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(EMSCRIPTEN_DIR "/home/wangao/projects/emsdk/upstream")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/public")
add_executable(yajp main.cpp parser.cpp lexer.cpp)

set_target_properties(yajp PROPERTIES LINK_FLAGS "--bind")
target_include_directories(yajp PUBLIC
  $<BUILD_INTERFACE:${EMSCRIPTEN_DIR}/include >
)
find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(yajp PRIVATE magic_enum::magic_enum)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(yajp PRIVATE fmt::fmt)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
