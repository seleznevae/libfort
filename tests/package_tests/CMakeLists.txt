cmake_minimum_required(VERSION 3.0)

find_package(libfort)

# Assert exports of libfort
if (NOT libfort_FOUND)
	message(FATAL_ERROR "libfort was not found")
endif()
if (NOT TARGET libfort::fort)
	message(FATAL_ERROR "TARGET libfort::fort not found")
endif()
if (NOT DEFINED libfort_LIBRARIES)
	message(FATAL_ERROR "libfort_LIBRARIES are not defined")
endif()
if (NOT ${libfort_VERSION} EQUAL "0.4.0")
	message(FATAL_ERROR "libfort_VERSION is incorrect")
endif()

add_executable(app 
    1-simple_table.cpp)

target_link_libraries(app
    libfort::fort)
