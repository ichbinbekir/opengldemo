cmake_minimum_required(VERSION 4.3)

project(opengldemo CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)

add_subdirectory(vendor/glad)
add_subdirectory(vendor/imgui)

file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include vendor/filewatch)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw spdlog::spdlog_header_only glad imgui)
