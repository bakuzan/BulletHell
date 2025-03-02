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
