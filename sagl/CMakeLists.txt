cmake_minimum_required(VERSION 3.13)

project(sagl
    VERSION 0.4
    DESCRIPTION "Parse table generator"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(grammar_parser STATIC src/grammar-parser/grammar_parser.cpp)

target_include_directories(grammar_parser PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../tools)

add_library(sagl SHARED src/sagl.cxx src/grammar-parser/grammar_parser.cpp)

target_include_directories(sagl PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/../tools)

add_executable(main src/main.cpp src/sagl/tabulate.cpp)

list(APPEND INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR} $)

target_include_directories(main PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../tools)

add_executable(json src/json.cpp src/sagl/tabulate.cpp)

target_include_directories(json PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../tools)

target_link_libraries(json PUBLIC grammar_parser)

add_executable(grammar src/grammar-parser/grammar.cpp src/sagl/tabulate.cpp)

target_include_directories(grammar PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../tools)

target_link_libraries(grammar PUBLIC grammar_parser)

set_property(TARGET main PROPERTY CXX_STANDARD 17)
set_property(TARGET json PROPERTY CXX_STANDARD 17)
set_property(TARGET grammar PROPERTY CXX_STANDARD 17)
set_property(TARGET grammar_parser PROPERTY CXX_STANDARD 17)
set_property(TARGET sagl PROPERTY CXX_STANDARD 17)

enable_testing()
add_test("main test" "python3 tests/test.py")
