option(FORT_HAVE_WCHAR "Enable wchar support" ON)
option(FORT_TEST_BUILD "Export some internal symbols for tests" ON)

add_library(fort_dev
    fort_impl.c
    vector.c
    string_buffer.c
    properties.c
    cell.c
    row.c
    table.c
    fort_utils.c
    wcwidth.c)

target_include_directories(fort_dev
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(FORT_HAVE_WCHAR)
    target_compile_definitions(fort_dev
        PRIVATE
            -DFT_CONGIG_HAVE_WCHAR
    )
endif()

if(FORT_TEST_BUILD)
    target_compile_definitions(fort_dev
        PRIVATE
            -DFT_TEST_BUILD=1
    )
endif()
