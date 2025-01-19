cmake_minimum_required(VERSION 3.30)

project(PhysicsSimulator)


set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)



file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/SDL2)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} SDL2 SDL2main)