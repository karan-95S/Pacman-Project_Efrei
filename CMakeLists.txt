cmake_minimum_required(VERSION 3.29)
project(pacman C)

set(CMAKE_C_STANDARD 23)

set (SDL2_DIR ./)

file(COPY bin/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(
        SRCS
        src/main.c
        src/framework.c
        src/firstLevel.c
        src/pacman.c
)

set(
        HEADER
        src/main.h
        src/framework.h
        src/firstLevel.h
        src/pacman.h
        src/spirits.c
        src/spirits.h
        src/superpacgum.h
        src/superpacgum.c
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(pacman WIN32 ${SRCS} ${HEADER})

# add_executable pour unix
# add_executable(pacman ${SRCS} ${HEADER})
target_link_libraries(pacman ${SDL2_LIBRARIES})
