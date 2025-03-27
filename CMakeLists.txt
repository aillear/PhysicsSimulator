cmake_minimum_required(VERSION 3.30)

project(PhysicsSimulator)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(PROJECT_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_PATH}/bin)

file(GLOB SRC 
    ${PROJECT_ROOT_PATH}/src/*.cpp
    ${PROJECT_ROOT_PATH}/src/core/*.cpp
    ${PROJECT_ROOT_PATH}/src/utils/*.cpp
    ${PROJECT_ROOT_PATH}/src/physics/*.cpp
    ${PROJECT_ROOT_PATH}/src/objects/*.cpp
    ${PROJECT_ROOT_PATH}/src/systems/*.cpp
)

# package
set(SDL2_DIR "${PROJECT_ROOT_PATH}/extern/SDL2/lib/cmake/SDL2")
set(SDL2_ttf_DIR "${PROJECT_ROOT_PATH}/extern/SDL2_ttf/lib/cmake/SDL2_ttf")
set(SDL2_image_DIR "${PROJECT_ROOT_PATH}/extern/SDL2_image/lib/cmake/SDL2_image")
set(SDL2_gfx_DIR "${PROJECT_ROOT_PATH}/extern/SDL2_gfx/lib/cmake/SDL2_gfx")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} 
    PRIVATE
    ${PROJECT_ROOT_PATH}/include/core
    ${PROJECT_ROOT_PATH}/include/utils
    ${PROJECT_ROOT_PATH}/include/physics
    ${PROJECT_ROOT_PATH}/include/objects
    ${PROJECT_ROOT_PATH}/include/systems
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    SDL2::SDL2 
    SDL2::SDL2main 
    SDL2_ttf::SDL2_ttf 
    SDL2_image::SDL2_image
    SDL2_gfx::SDL2_gfx
)

# add_subdirectory(test)
# option(BUILD_TESTS "Build unit tests" ON)
# if(BUILD_TESTS)
#     add_subdirectory(test)
# endif()