cmake_minimum_required(VERSION 3.17)
project(tanks_2d_udp)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

add_executable(tanks_2d_udp game.cpp)

target_link_libraries(tanks_2d_udp ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
