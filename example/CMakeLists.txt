
add_executable(${PROJECT_NAME}_simple_table
    simple_table.c)
target_link_libraries(${PROJECT_NAME}_simple_table
    fort)

add_executable(${PROJECT_NAME}_custom_table
    custom_table.c)
target_link_libraries(${PROJECT_NAME}_custom_table
    fort)

add_executable(${PROJECT_NAME}_fill_table
    fill_table.c)
target_link_libraries(${PROJECT_NAME}_fill_table
    fort)

add_executable(${PROJECT_NAME}_custom_border_style
    custom_border_style.c)
target_link_libraries(${PROJECT_NAME}_custom_border_style
    fort)

add_executable(${PROJECT_NAME}_print_styles
    print_styles.c)
target_link_libraries(${PROJECT_NAME}_print_styles
    fort)

add_executable(${PROJECT_NAME}_math_table
    math_table.c)
target_link_libraries(${PROJECT_NAME}_math_table
    fort)

add_executable(${PROJECT_NAME}_beautiful_table
    beautiful_table.c)
target_link_libraries(${PROJECT_NAME}_beautiful_table
    fort)

add_executable(${PROJECT_NAME}_complex_layout
    complex_layout.c)
target_link_libraries(${PROJECT_NAME}_complex_layout
    fort)

add_executable(${PROJECT_NAME}_non_ascii_table
    non_ascii_table.c)
target_link_libraries(${PROJECT_NAME}_non_ascii_table
    fort)




add_executable(${PROJECT_NAME}_ex_cpp
    main.cpp)
target_link_libraries(${PROJECT_NAME}_ex_cpp
    fort)

set(${PROJECT_NAME}_examples
    ${PROJECT_NAME}_simple_table
    ${PROJECT_NAME}_custom_table
    ${PROJECT_NAME}_fill_table
    ${PROJECT_NAME}_custom_border_style
    ${PROJECT_NAME}_print_styles
    ${PROJECT_NAME}_math_table
    ${PROJECT_NAME}_beautiful_table
    ${PROJECT_NAME}_complex_layout
    ${PROJECT_NAME}_non_ascii_table
    ${PROJECT_NAME}_ex_cpp
    PARENT_SCOPE)

if(DEFINED FORT_LINK_LIBRARIES)
    foreach(exe ${${PROJECT_NAME}_examples})
        target_link_libraries(${exe} "${FORT_LINK_LIBRARIES}")
    endforeach()
endif()
