project(libfort)

# Required cmake version
cmake_minimum_required(VERSION 2.8)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# Built options
option(FORT_CXX_BUILD "Compile with c++ compiler instead of c" OFF)
option(FORT_ENABLE_ASTYLE "Enable astyle" OFF)
set(FORT_BUILD_TYPE "common" CACHE STRING "Build types(common, asan, ubsan, coveralls)")


# Determine compiler (pos. values Clang, GNU, Intel, MSVC, AppleClang...
# (https://cmake.org/cmake/help/v3.0/variable/CMAKE_LANG_COMPILER_ID.html)
if(FORT_CXX_BUILD)
    set(FORT_COMPILER ${CMAKE_CXX_COMPILER_ID})
else(FORT_CXX_BUILD)
    set(FORT_COMPILER ${CMAKE_C_COMPILER_ID})
endif(FORT_CXX_BUILD)



set(CMAKE_VERBOSE_MAKEFILE ON)


# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include_directories(include)
include_directories(src)




# ------------------------------------------------------------------------------
# Warnings
# ------------------------------------------------------------------------------

if(FORT_COMPILER STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W4")
else(FORT_COMPILER STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")
endif(FORT_COMPILER STREQUAL "MSVC")





# ------------------------------------------------------------------------------
# Sources and executables
# ------------------------------------------------------------------------------

FILE(GLOB_RECURSE FortHeaders "include/*.h" "include/*.hpp" "tests/*.h" "src/*.h")
add_custom_target(headers SOURCES ${FortHeaders})


set(FORT_SOURCES
    src/fort.c
    src/vector.c
    src/string_buffer.c
    src/options.c
    src/cell.c
    src/row.c
    src/table.c
    src/fort_impl.c
    src/wcwidth.c)




set(EXAMPLE_SOURCES
    example/main.c)
add_executable(${PROJECT_NAME}_example
    ${EXAMPLE_SOURCES}
    ${FORT_SOURCES})

set(EXAMPLE_CPP_SOURCES
    example/main.cpp)
add_executable(${PROJECT_NAME}_example_cpp
    ${EXAMPLE_CPP_SOURCES}
    ${FORT_SOURCES})

set(TEST_SOURCES
    tests/test.c
    tests/test_vector.c
    tests/test_string_buffer.c
    tests/test_table_geometry.c
    tests/test_table_basic.c
    tests/test_table_border_style.c
    tests/test_table_options.c
    tests/test_utility.c)
add_executable(${PROJECT_NAME}_test
    ${FORT_SOURCES}
    ${TEST_SOURCES})


if(FORT_CXX_BUILD)
    SET_SOURCE_FILES_PROPERTIES( ${FORT_SOURCES} PROPERTIES LANGUAGE CXX)
    SET_SOURCE_FILES_PROPERTIES( ${EXAMPLE_SOURCES} PROPERTIES LANGUAGE CXX)
    SET_SOURCE_FILES_PROPERTIES( ${TEST_SOURCES} PROPERTIES LANGUAGE CXX)
endif(FORT_CXX_BUILD)



# ------------------------------------------------------------------------------
# Sanitizers
# ------------------------------------------------------------------------------

if(FORT_COMPILER STREQUAL "GNU" OR FORT_COMPILER STREQUAL "Clang")
    # asan case
    if(FORT_BUILD_TYPE STREQUAL "asan")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
        set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -fsanitize=address")
        if(FORT_COMPILER STREQUAL "GNU")
            target_link_libraries(${PROJECT_NAME}_example asan)
            target_link_libraries(${PROJECT_NAME}_test asan)
        endif(FORT_COMPILER STREQUAL "GNU")
        target_link_libraries(${PROJECT_NAME}_example_cpp asan)
    endif(FORT_BUILD_TYPE STREQUAL "asan")

    #ubsan case
    if(FORT_BUILD_TYPE STREQUAL "ubsan")
        # -fno-sanitize-recover option is used to force faild
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fno-sanitize-recover")
        set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -fsanitize=undefined -fno-sanitize-recover")
        if(FORT_COMPILER STREQUAL "GNU")
            target_link_libraries(${PROJECT_NAME}_example ubsan)
            target_link_libraries(${PROJECT_NAME}_example_cpp ubsan)
            target_link_libraries(${PROJECT_NAME}_test ubsan)
        endif(FORT_COMPILER STREQUAL "GNU")
    endif(FORT_BUILD_TYPE STREQUAL "ubsan")

    #coveralls case
    if(FORT_BUILD_TYPE STREQUAL "coveralls")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage")
        set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -g -fprofile-arcs -ftest-coverage")
        target_link_libraries(${PROJECT_NAME}_example_cpp gcov)
    endif(FORT_BUILD_TYPE STREQUAL "coveralls")

endif(FORT_COMPILER STREQUAL "GNU" OR FORT_COMPILER STREQUAL "Clang")


# Hack for some gcc versions
if(FORT_COMPILER STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
endif(FORT_COMPILER STREQUAL "GNU")



# ------------------------------------------------------------------------------
# Astyle
# ------------------------------------------------------------------------------

if(FORT_ENABLE_ASTYLE)
    list(APPEND ASTYLE_CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
    )

    ExternalProject_Add(
        astyle
        GIT_REPOSITORY      https://github.com/Bareflank/astyle.git
        GIT_TAG             v1.2
        GIT_SHALLOW         1
        CMAKE_ARGS          ${ASTYLE_CMAKE_ARGS}
        PREFIX              ${CMAKE_BINARY_DIR}/external/astyle/prefix
        TMP_DIR             ${CMAKE_BINARY_DIR}/external/astyle/tmp
        STAMP_DIR           ${CMAKE_BINARY_DIR}/external/astyle/stamp
        DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/astyle/download
        SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/astyle/src
        BINARY_DIR          ${CMAKE_BINARY_DIR}/external/astyle/build
    )

    list(APPEND ASTYLE_ARGS
        --style=kr
        --lineend=linux
        --suffix=none
        --pad-oper
        --unpad-paren
        --align-pointer=name
        --align-reference=name
        --indent-switches
        --keep-one-line-statements
        --keep-one-line-blocks
        --pad-header
        --convert-tabs
        --min-conditional-indent=0
        --indent=spaces=4
        ${CMAKE_SOURCE_DIR}/include/*.h
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/tests/*.c
    )

    if(NOT WIN32 STREQUAL "1")
        add_custom_target(
            format
            COMMAND ${CMAKE_BINARY_DIR}/bin/astyle ${ASTYLE_ARGS}
            COMMENT "running astyle"
        )
    else()
        add_custom_target(
            format
            COMMAND ${CMAKE_BINARY_DIR}/bin/astyle.exe ${ASTYLE_ARGS}
            COMMENT "running astyle"
        )
    endif()

endif()
