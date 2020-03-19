cmake_minimum_required(VERSION 3.13)

project(sagl
    VERSION 0.5
    DESCRIPTION "Parse table generator"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(grammar_parser STATIC src/sagl/grammar-parser/grammar_parser.cpp)

target_include_directories(grammar_parser PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src)

add_library(sagl SHARED src/sagl/sagl.cxx src/sagl/grammar-parser/grammar_parser.cpp src/sagl/internals/tabulate.cpp)

target_include_directories(sagl PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src)

add_executable(example examples/example.cpp src/sagl/internals/tabulate.cpp)

list(APPEND INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR} $)

target_include_directories(example PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include)

set_property(TARGET example PROPERTY CXX_STANDARD 17)
set_property(TARGET grammar_parser PROPERTY CXX_STANDARD 17)
set_property(TARGET sagl PROPERTY CXX_STANDARD 17)

enable_testing()
add_test("example test" "python3 tests/test.py")

install(TARGETS sagl DESTINATION lib)
install(FILES include/sagl.h DESTINATION include)

target_compile_options(example PUBLIC -g -Wall -Wextra -Wpedantic)
