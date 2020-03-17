cmake_minimum_required(VERSION 3.13)

project(
    rnd
    VERSION 0.4
    DESCRIPTION "Convert regular expression to finite state machine"
    LANGUAGES CXX C
)

add_library(regexp SHARED
	src/rnd/automaton.cpp
	src/rnd/nexpr.cpp
	src/rnd/zsymbols.cpp
	src/regexp/regexp.cxx
    ../sagl/src/sagl/tabulate.cpp)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PUBLIC_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
list(APPEND PRIVATE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src")
list(APPEND PRIVATE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/regexp")
list(APPEND PRIVATE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../tools")
list(APPEND PRIVATE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../sagl/include")

target_include_directories(regexp
    PUBLIC ${PUBLIC_INCLUDE_DIRS}
    PRIVATE ${PRIVATE_INCLUDE_DIRS})

install(TARGETS regexp DESTINATION lib)
install(FILES include/regexp.h DESTINATION include)

add_executable(nexpr.test
	src/rnd/nexpr.cpp
	src/rnd/nexpr.test.cpp
	src/rnd/zsymbols.cpp)

target_include_directories(nexpr.test
    PUBLIC ${PUBLIC_INCLUDE_DIRS}
    PRIVATE ${PRIVATE_INCLUDE_DIRS})

add_executable(zsymbols.test
	src/rnd/zsymbols.cpp
	src/rnd/zsymbols.test.cpp)

target_include_directories(zsymbols.test
    PUBLIC ${PUBLIC_INCLUDE_DIRS}
    PRIVATE ${PRIVATE_INCLUDE_DIRS})

add_executable(regexp.test
    src/regexp/regexp.test.c
	src/regexp/regexp.cxx
	src/rnd/automaton.cpp
    src/rnd/nexpr.cpp
    src/rnd/zsymbols.cpp
    ../sagl/src/sagl/tabulate.cpp)

target_include_directories(regexp.test
    PUBLIC ${PUBLIC_INCLUDE_DIRS}
    PRIVATE ${PRIVATE_INCLUDE_DIRS})

add_executable(automaton.test
	src/rnd/automaton.test.cpp
	src/rnd/automaton.cpp
	src/rnd/minauto.cpp
	src/rnd/nexpr.cpp
	src/rnd/zsymbols.cpp)

target_include_directories(automaton.test
    PUBLIC ${PUBLIC_INCLUDE_DIRS}
    PRIVATE ${PRIVATE_INCLUDE_DIRS})

set_property(TARGET regexp PROPERTY CXX_STANDARD 17)
set_property(TARGET nexpr.test PROPERTY CXX_STANDARD 17)
set_property(TARGET regexp.test PROPERTY CXX_STANDARD 17)
set_property(TARGET zsymbols.test PROPERTY CXX_STANDARD 17)
set_property(TARGET automaton.test PROPERTY CXX_STANDARD 17)

enable_testing()
add_test(nexpr.test nexpr.test)
add_test(automaton.test automaton.test)
add_test(zsymbols.test zsymbols.test)
add_test(regexp.test regexp.test)
