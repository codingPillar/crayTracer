cmake_minimum_required(VERSION 3.2)
project(sceneRayTracer)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

include_directories("include")

FILE(GLOB SRC sdlView/* src/*)

add_executable(tracer ${SRC})
target_link_libraries(tracer m)

target_include_directories(tracer PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(tracer ${SDL2_LIBRARIES})
target_compile_options(tracer PUBLIC ${SDL2_CFLAGS_OTHER})