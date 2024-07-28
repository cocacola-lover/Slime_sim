cmake_minimum_required(VERSION 3.0.0)
project(Test VERSION 0.1.0 LANGUAGES C)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(
    Test 
    main.c 
    src/agent_field.c
    src/trace_map.c
)
target_link_libraries(Test ${SDL2_LIBRARIES})
target_link_libraries(Test m)

