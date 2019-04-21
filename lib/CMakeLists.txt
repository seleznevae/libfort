option(FORT_HAVE_WCHAR "Enable wchar support" ON)

message("FORT_HAVE_WCHAR in fort = ${FORT_HAVE_WCHAR}")
add_library(fort
    fort.c)

target_include_directories(fort
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(FORT_HAVE_WCHAR)
    target_compile_definitions(fort
        PRIVATE
            -DFT_CONGIG_HAVE_WCHAR
    )
endif()
