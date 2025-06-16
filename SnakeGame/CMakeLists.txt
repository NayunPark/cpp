cmake_minimum_required(VERSION 3.16)
project(SnakeGame LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)

file(GLOB SRC "src/*.cpp")
add_executable(SnakeGame ${SRC})

target_include_directories(SnakeGame PRIVATE include)
target_link_libraries(SnakeGame PRIVATE ${CURSES_LIBRARIES})

