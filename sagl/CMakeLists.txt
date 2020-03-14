cmake_minimum_required(VERSION 3.13)

project(sagl
    VERSION 0.4
    DESCRIPTION "Parse table generator"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main src/main.cpp)

list(APPEND INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR} $)

target_include_directories(main PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../tools)

add_executable(json src/json.cpp)

target_include_directories(json PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../tools)

set_property(TARGET main PROPERTY CXX_STANDARD 17)
set_property(TARGET json PROPERTY CXX_STANDARD 17)

enable_testing()
add_test("main test" "python3 tests/test.py")
