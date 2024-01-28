cmake_minimum_required(VERSION 3.2)
project(sceneRayTracer)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories("include")

FILE(GLOB SRC src/*)

add_executable(tracer ${SRC})
target_link_libraries(tracer m)
