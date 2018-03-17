project(libfort)

# Required cmake version
cmake_minimum_required(VERSION 2.8)


option(FORT_TEST_BUILD "Test build with sanitizers and small library stack size" ON)
option(FORT_GCC_BUILD "Build with gcc" ON)
option(FORT_MSVC_BUILD "Build with msvc" OFF)
option(FORT_CXX_BUILD "Compile with c++ compiler instead of c" OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)
include_directories(src)

if(FORT_MSVC_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W4")
else(FORT_MSVC_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")
endif(FORT_MSVC_BUILD)








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

if(FORT_CXX_BUILD)
    SET_SOURCE_FILES_PROPERTIES( ${FORT_SOURCES} PROPERTIES LANGUAGE CXX)
endif(FORT_CXX_BUILD)





add_executable(${PROJECT_NAME}_example
    example/main.c
    ${FORT_SOURCES})



add_executable(${PROJECT_NAME}_test
    ${FORT_SOURCES}
    tests/test.c
    tests/test_vector.c
    tests/test_table.c
    tests/test_string_buffer.c)



if(FORT_TEST_BUILD AND NOT FORT_MSVC_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    if(FORT_GCC_BUILD)
        target_link_libraries(${PROJECT_NAME}_example asan)
        target_link_libraries(${PROJECT_NAME}_test asan)
    endif(FORT_GCC_BUILD)
endif(FORT_TEST_BUILD AND NOT FORT_MSVC_BUILD)

