cmake_minimum_required(VERSION 3.16)
project(tanks_2d_udp)

set(CMAKE_CXX_STANDARD 20)

set(CLIENT_MODULES game.cpp objects.cpp game_map.cpp cam.cpp game_session.cpp client.cpp messages.cpp tinyxml2.cpp map.cpp menu.cpp statistics.cpp)

set(SERVER_MODULES server.cpp messages.cpp)

set(MODULES_PATH "project/src/")

set(INCLUDE_PATH "project/includes/")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 COMPONENTS system window graphics audio network REQUIRED)

list(TRANSFORM CLIENT_MODULES PREPEND ${MODULES_PATH})

list(TRANSFORM SERVER_MODULES PREPEND ${MODULES_PATH})

include_directories(${SFML_INCLUDE_DIR} ${INCLUDE_PATH})

add_executable(game.out ${CLIENT_MODULES})

add_executable(server.out ${SERVER_MODULES})

target_link_libraries(game.out ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

target_link_libraries(server.out ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})