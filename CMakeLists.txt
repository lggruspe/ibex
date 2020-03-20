cmake_minimum_required(VERSION 3.13)

project(ibex
    VERSION 0.5
    DESCRIPTION "Parser generator"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(rnd SHARED
	src/rnd/internals/automaton.cpp
	src/rnd/internals/nexpr.cpp
	src/rnd/internals/zsymbols.cpp
	src/rnd/rnd.cxx)

set(RND_PUBLIC_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
set(RND_PRIVATE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src")

target_include_directories(rnd
    PUBLIC "${RND_PUBLIC_INCLUDE_DIRS}"
    PRIVATE "${RND_PRIVATE_INCLUDE_DIRS}")

add_executable(nexpr.test
	src/rnd/internals/nexpr.cpp
	src/rnd/internals/nexpr.test.cpp
	src/rnd/internals/zsymbols.cpp)

target_include_directories(nexpr.test
    PUBLIC "${RND_PUBLIC_INCLUDE_DIRS}"
    PRIVATE "${RND_PRIVATE_INCLUDE_DIRS}")

add_executable(zsymbols.test
	src/rnd/internals/zsymbols.cpp
	src/rnd/internals/zsymbols.test.cpp)

target_include_directories(zsymbols.test
    PUBLIC "${RND_PUBLIC_INCLUDE_DIRS}"
    PRIVATE "${RND_PRIVATE_INCLUDE_DIRS}")

add_executable(rnd.test
    src/rnd/rnd.test.c
	src/rnd/rnd.cxx
	src/rnd/internals/automaton.cpp
    src/rnd/internals/nexpr.cpp
    src/rnd/internals/zsymbols.cpp)

target_include_directories(rnd.test
    PUBLIC "${RND_PUBLIC_INCLUDE_DIRS}"
    PRIVATE "${RND_PRIVATE_INCLUDE_DIRS}")

add_executable(automaton.test
	src/rnd/internals/automaton.test.cpp
	src/rnd/internals/automaton.cpp
	src/rnd/internals/minauto.cpp
	src/rnd/internals/nexpr.cpp
	src/rnd/internals/zsymbols.cpp)

target_include_directories(automaton.test
    PUBLIC "${RND_PUBLIC_INCLUDE_DIRS}"
    PRIVATE "${RND_PRIVATE_INCLUDE_DIRS}")

add_library(sagl SHARED src/sagl/sagl.cxx src/sagl/internals/tabulate.cpp)

target_include_directories(sagl PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src)

add_executable(example examples/example.cpp src/sagl/internals/tabulate.cpp)

list(APPEND INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR} $)

target_include_directories(example PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include)

set_property(TARGET rnd PROPERTY CXX_STANDARD 17)
set_property(TARGET nexpr.test PROPERTY CXX_STANDARD 17)
set_property(TARGET rnd.test PROPERTY CXX_STANDARD 17)
set_property(TARGET zsymbols.test PROPERTY CXX_STANDARD 17)
set_property(TARGET automaton.test PROPERTY CXX_STANDARD 17)
set_property(TARGET example PROPERTY CXX_STANDARD 17)
set_property(TARGET sagl PROPERTY CXX_STANDARD 17)

enable_testing()
add_test(nexpr.test nexpr.test)
add_test(automaton.test automaton.test)
add_test(zsymbols.test zsymbols.test)
add_test(rnd.test rnd.test)
add_test("example test" "python3 tests/test.py")

install(TARGETS rnd DESTINATION lib)
install(FILES include/rnd.h DESTINATION include)
install(TARGETS sagl DESTINATION lib)
install(FILES include/sagl.h DESTINATION include)

target_compile_options(rnd PUBLIC -g -Wall -Wextra -Wpedantic -Werror)
target_compile_options(nexpr.test PUBLIC -g -Wall -Wextra -Wpedantic -Werror)
target_compile_options(rnd.test PUBLIC -g -Wall -Wextra -Wpedantic -Werror)
target_compile_options(zsymbols.test  PUBLIC -g -Wall -Wextra -Wpedantic -Werror)
target_compile_options(automaton.test PUBLIC -g -Wall -Wextra -Wpedantic -Werror)
target_compile_options(example PUBLIC -g -Wall -Wextra -Wpedantic -Werror)
target_compile_options(sagl PUBLIC -g -Wall -Wextra -Wpedantic -Werror)
