
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

set(${PROJECT_NAME}_examples
    ${PROJECT_NAME}_ex
    ${PROJECT_NAME}_print_styles
    ${PROJECT_NAME}_ex_cpp
    PARENT_SCOPE)

if(DEFINED FORT_LINK_LIBRARIES)
    foreach(exe ${${PROJECT_NAME}_examples})
        target_link_libraries(${exe} "${FORT_LINK_LIBRARIES}")
    endforeach()
endif()
