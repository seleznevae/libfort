get_filename_component(CMOCKA_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

if (EXISTS "${CMOCKA_CMAKE_DIR}/CMakeCache.txt")
    # In build tree
    include(${CMOCKA_CMAKE_DIR}/cmocka-build-tree-settings.cmake)
else()
    set(CMOCKA_INCLUDE_DIR /usr/local/include)
endif()

set(CMOCKA_LIBRARY /usr/local/lib/libcmocka.so)
set(CMOCKA_LIBRARIES /usr/local/lib/libcmocka.so)
