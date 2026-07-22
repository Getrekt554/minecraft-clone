cmake_minimum_required(VERSION 3.20)

project(MinecraftClone)

add_subdirectory(external/glfw)

add_library(glad external/glad/src/glad.c)

target_include_directories(glad PUBLIC external/glad/include)

find_package(OpenGL REQUIRED)

add_executable(main src/main.cpp src/camera.cpp src/renderer.cpp src/world.cpp src/utilities.cpp src/block_handler.cpp)

target_link_libraries(main PRIVATE glfw glad OpenGL::GL)

target_include_directories(main PRIVATE include)
