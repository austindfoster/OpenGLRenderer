cmake_minimum_required(VERSION 4.2)

project(opengl_renderer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

add_executable(opengl_renderer 
    src/config.h 
    src/glad.c
    src/camera.cpp
    src/light.cpp
    src/loadTextures.cpp
    src/mesh.cpp
    src/model.cpp
    src/shader.cpp
    src/stb.cpp
    src/main.cpp)

target_include_directories(opengl_renderer PRIVATE include dependencies)

target_link_libraries(opengl_renderer glfw OpenGL::GL assimp::assimp)