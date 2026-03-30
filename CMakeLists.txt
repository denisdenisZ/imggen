cmake_minimum_required(VERSION 3.20)
project(imgen C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(src)



if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-ggdb>
    $<$<CONFIG:Release>:-O2>
)

# target_include_directories(${PROJECT_NAME} PRIVATE include)
