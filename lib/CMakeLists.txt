option(FORT_HAVE_WCHAR "Enable wchar support" ON)

add_library(fort
    fort.c)

target_include_directories(fort
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(fort
    PROPERTIES VERSION 0.1.1
    SOVERSION 0.1)

if(FORT_HAVE_WCHAR)
    target_compile_definitions(fort
        PRIVATE
            -DFT_CONGIG_HAVE_WCHAR
    )
endif()


include(GNUInstallDirs)
install(
    TARGETS
      fort
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES fort.h fort.hpp DESTINATION include)
