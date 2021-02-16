cmake_minimum_required(VERSION 3.17)
project(Year1_CPP_Project)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_INCLUDE_DIR C:/libs/SDL2/include)
set(SDL2_LIB_DIR C:/libs/SDL2/lib/x86)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(Year1_CPP_Project main.cpp)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)