cmake_minimum_required(VERSION 3.30)

project(PhysicsSimulator)

set(PROJECT_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_PATH}/bin)

file(GLOB_RECURSE SRC ${PROJECT_ROOT_PATH}/src/*.cpp)


set(SDL2_DIR "${PROJECT_ROOT_PATH}/extern/SDL2/lib/cmake/SDL2")
set(SDL2_ttf_DIR "${PROJECT_ROOT_PATH}/extern/SDL2_ttf/lib/cmake/SDL2_ttf")
set(SDL2_image_DIR "${PROJECT_ROOT_PATH}/extern/SDL2_image/lib/cmake/SDL2_image")
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image)