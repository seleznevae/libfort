
add_executable(${PROJECT_NAME}_ex
    main.c)
target_link_libraries(${PROJECT_NAME}_ex
    fort)

add_executable(${PROJECT_NAME}_print_styles
    print_styles.c)
target_link_libraries(${PROJECT_NAME}_print_styles
    fort)

add_executable(${PROJECT_NAME}_ex_cpp
    main.cpp)
target_link_libraries(${PROJECT_NAME}_ex_cpp
    fort)

if(DEFINED FORT_LINK_LIBRARIES)
    target_link_libraries(${PROJECT_NAME}_ex "${FORT_LINK_LIBRARIES}")
    target_link_libraries(${PROJECT_NAME}_print_styles "${FORT_LINK_LIBRARIES}")
    target_link_libraries(${PROJECT_NAME}_ex_cpp "${FORT_LINK_LIBRARIES}")
endif()
