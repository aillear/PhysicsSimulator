cmake_minimum_required(VERSION 3.30)

project(PhysicsSimulator VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(PROJECT_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_PATH}/bin)
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# if (MSVC)
#     add_compile_options(/W4 /WX)
# else()
#     add_compile_options(-Wall -Wextra -pedantic -Werror)
# endif()

file(GLOB SRC 
    ${PROJECT_ROOT_PATH}/src/*.cpp
    ${PROJECT_ROOT_PATH}/src/UI/*.cpp
    ${PROJECT_ROOT_PATH}/src/core/*.cpp
    ${PROJECT_ROOT_PATH}/src/utils/*.cpp
    ${PROJECT_ROOT_PATH}/src/physics/*.cpp
    ${PROJECT_ROOT_PATH}/src/objects/*.cpp
    ${PROJECT_ROOT_PATH}/src/systems/*.cpp
    ${PROJECT_ROOT_PATH}/src/UI/element/*.cpp
)

# package
set(glm_DIR "${PROJECT_ROOT_PATH}/extern/glm/share/glm")
set(SDL3_DIR "${PROJECT_ROOT_PATH}/extern/SDL3/lib/cmake/SDL3")
set(SDL3_ttf_DIR "${PROJECT_ROOT_PATH}/extern/SDL3_ttf/lib/cmake/SDL3_ttf")
set(SDL3_gfx_DIR "${PROJECT_ROOT_PATH}/extern/SDL3_gfx/lib/cmake/SDL3_gfx")
set(SDL3_image_DIR "${PROJECT_ROOT_PATH}/extern/SDL3_image/lib/cmake/SDL3_image")

find_package(glm REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_gfx REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(${TARGET} ${SRC})

target_include_directories(${TARGET}
    PRIVATE
    ${PROJECT_ROOT_PATH}/include/UI
    ${PROJECT_ROOT_PATH}/include/core
    ${PROJECT_ROOT_PATH}/include/utils
    ${PROJECT_ROOT_PATH}/include/physics
    ${PROJECT_ROOT_PATH}/include/objects
    ${PROJECT_ROOT_PATH}/include/systems
    ${PROJECT_ROOT_PATH}/include/UI/element
)

target_link_libraries(${TARGET}
    PRIVATE
    glm::glm
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_gfx::SDL3_gfx
    SDL3_image::SDL3_image
)

# add_subdirectory(test)
# option(BUILD_TESTS "Build unit tests" ON)
# if(BUILD_TESTS)
#     add_subdirectory(test)
# endif()