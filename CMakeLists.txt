project(libfort)

# Required cmake version
cmake_minimum_required(VERSION 2.8)

# Built options
option(FORT_CXX_BUILD "Compile with c++ compiler instead of c" OFF)
#option(FORT_COVERALLS_BUILD "Build for coveralls" OFF)
set(FORT_BUILD_TYPE "common" CACHE STRING "Built types (possible values: common, asan, ubsan, coveralls)")
#set(FORT_COMPILER "gcc" CACHE STRING "Compiler (possible values: gcc, clang, msvc)")


# Determine compiler (pos. values Clang, GNU, Intel, MSVC, AppleClang... (https://cmake.org/cmake/help/v3.0/variable/CMAKE_LANG_COMPILER_ID.html)
if(FORT_CXX_BUILD)
    set(FORT_COMPILER ${CMAKE_CXX_COMPILER_ID})
else(FORT_CXX_BUILD)
    set(FORT_COMPILER ${CMAKE_C_COMPILER_ID})
endif(FORT_CXX_BUILD)



set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)
include_directories(src)


# Turn on warnings
if(FORT_COMPILER STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W4")
else(FORT_COMPILER STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")
endif(FORT_COMPILER STREQUAL "MSVC")






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



set(EXAMPLE_SOURCES
    example/main.c)
add_executable(${PROJECT_NAME}_example
    ${EXAMPLE_SOURCES}
    ${FORT_SOURCES})


set(TEST_SOURCES
    tests/test.c
    tests/test_vector.c
    tests/test_table.c
    tests/test_string_buffer.c)
add_executable(${PROJECT_NAME}_test
    ${FORT_SOURCES}
    ${TEST_SOURCES})


if(FORT_CXX_BUILD)
    SET_SOURCE_FILES_PROPERTIES( ${FORT_SOURCES} PROPERTIES LANGUAGE CXX)
    SET_SOURCE_FILES_PROPERTIES( ${EXAMPLE_SOURCES} PROPERTIES LANGUAGE CXX)
    SET_SOURCE_FILES_PROPERTIES( ${TEST_SOURCES} PROPERTIES LANGUAGE CXX)
endif(FORT_CXX_BUILD)




# Adding sanitizers
if(FORT_COMPILER STREQUAL "GNU" OR FORT_COMPILER STREQUAL "Clang")
    # asan case
    if(FORT_BUILD_TYPE STREQUAL "asan")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
        if(FORT_COMPILER STREQUAL "GNU")
            target_link_libraries(${PROJECT_NAME}_example asan)
            target_link_libraries(${PROJECT_NAME}_test asan)
        endif(FORT_COMPILER STREQUAL "GNU")
    endif(FORT_BUILD_TYPE STREQUAL "asan")

    #ubsan case
    if(FORT_BUILD_TYPE STREQUAL "ubsan")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-sanitize-recover") #to force fail
        if(FORT_COMPILER STREQUAL "GNU")
            target_link_libraries(${PROJECT_NAME}_example ubsan)
            target_link_libraries(${PROJECT_NAME}_test ubsan)
        endif(FORT_COMPILER STREQUAL "GNU")
    endif(FORT_BUILD_TYPE STREQUAL "ubsan")

    #coveralls case
    if(FORT_BUILD_TYPE STREQUAL "coveralls")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage")
    endif(FORT_BUILD_TYPE STREQUAL "coveralls")

endif(FORT_COMPILER STREQUAL "GNU" OR FORT_COMPILER STREQUAL "Clang")


# Coveralls support
#if(FORT_COVERALLS_BUILD)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage")
#endif(FORT_COVERALLS_BUILD)
