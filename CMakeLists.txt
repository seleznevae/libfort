cmake_minimum_required(VERSION 2.8)

project(libfort)

option(FORT_TEST_BUILD "Test build with sanitizers and small library stack size" ON)
option(FORT_GCC_BUILD "Build with gcc" ON)
option(CERAII_MSVC_BUILD "Build with msvc" OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)
include_directories(src)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g ")




FILE(GLOB_RECURSE FortHeaders "include/*.h" "tests/*.h" "src/*.h" "src/*.h" "src/*.h" "src/*.h" "src/*.h" "src/*.h")
add_custom_target(headers SOURCES ${FortHeaders})


set(FORT_SOURCES
    src/fort.c
    src/vector.c
    src/string_buffer.c
    src/options.c
    src/cell.c
    src/row.c
    src/table.c
    src/fort_impl.c)


add_executable(${PROJECT_NAME}_example
    example/main.c
    ${FORT_SOURCES})


include_directories(tests/cmocka-1.1.0/include)
link_directories(${CMAKE_SOURCE_DIR}/tests/cmocka-1.1.0/build/src)

add_executable(${PROJECT_NAME}_test
    ${FORT_SOURCES}
    tests/test.c
    tests/test_vector.c
    tests/test_table.c
    tests/test_string_buffer.c)

target_link_libraries(libfort_test
    cmocka)


if(FORT_TEST_BUILD AND NOT CERAII_MSVC_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    if(FORT_GCC_BUILD)
        target_link_libraries(${PROJECT_NAME}_example asan)
        target_link_libraries(${PROJECT_NAME}_test asan)
    endif(FORT_GCC_BUILD)
endif(FORT_TEST_BUILD AND NOT CERAII_MSVC_BUILD)

