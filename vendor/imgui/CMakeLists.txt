project(imgui CXX)

file(GLOB SOURCES src/*.cpp)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
# target_link_libraries(${PROJECT_NAME} PUBLIC glad)
