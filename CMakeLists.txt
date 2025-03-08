cmake_minimum_required(VERSION 3.15)
project(CMake)

set(CMAKE_CXX_STANDARD 17)

# Include SFML
set(SFML_DIR "D:/projects/SFML/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Add your executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Copy SFML to build location
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/bin/sfml-system-d-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/bin/sfml-graphics-d-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/bin/sfml-window-d-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/bin/sfml-system-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/bin/sfml-graphics-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/bin/sfml-window-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

