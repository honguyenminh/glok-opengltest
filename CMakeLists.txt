cmake_minimum_required(VERSION 3.22.1)
project(glok)

set(CMAKE_CXX_STANDARD 23)

add_executable(glok src/main.cpp
        src/Renderer/Renderer.cpp
        src/Renderer/Renderer.h
        src/Renderer/VertexBuffer.cpp
        src/Renderer/VertexBuffer.h
        src/Renderer/IndexBuffer.cpp
        src/Renderer/IndexBuffer.h
        src/Renderer/VertexArray.cpp
        src/Renderer/VertexArray.h
        src/Renderer/VertexBufferLayout.cpp
        src/Renderer/VertexBufferLayout.h
        src/Renderer/Shader.cpp
        src/Renderer/Shader.h)

# libs include
target_include_directories(glok PUBLIC lib/include)
# GLFW
add_subdirectory(lib/glfw-3.4)
target_link_libraries(glok glfw)
# GLEW
add_compile_definitions(GLEW_STATIC)
target_link_libraries(glok ${CMAKE_SOURCE_DIR}/lib/GLEW/glew32s.lib)
# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(glok OpenGL::GL)