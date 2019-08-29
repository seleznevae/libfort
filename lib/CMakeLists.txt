option(FORT_HAVE_WCHAR "Enable wchar support" ON)
option(FORT_HAVE_UTF8 "Enable UTF8 support" ON)

add_library(fort
    fort.c)

target_include_directories(fort
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(fort
    PROPERTIES VERSION ${libfort_VERSION}
    SOVERSION ${libfort_SOVERSION})

if(NOT FORT_HAVE_WCHAR)
    target_compile_definitions(fort
        PRIVATE
            -DFT_CONGIG_DISABLE_WCHAR
    )
endif()

if(NOT FORT_HAVE_UTF8)
    target_compile_definitions(fort
        PRIVATE
            -DFT_CONGIG_DISABLE_UTF8
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

install(FILES fort.h fort.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(libfort.pc.in libfort.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libfort.pc DESTINATION lib/pkgconfig)
