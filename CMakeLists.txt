cmake_minimum_required(VERSION 3.16)
project(tanks_2d_udp)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(MODULES game.cpp objects.cpp game_map.cpp cam.cpp tinyxml2.cpp TmxLevel.cpp)

set(MODULES_PATH "project/src/")

set(INCLUDE_PATH "project/includes/")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)

list(TRANSFORM MODULES PREPEND ${MODULES_PATH})

include_directories(${SFML_INCLUDE_DIR} ${INCLUDE_PATH})

add_executable(tanks_2d_udp ${MODULES})

target_link_libraries(tanks_2d_udp ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})