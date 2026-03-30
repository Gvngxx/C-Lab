cmake_minimum_required(VERSION 3.10)
project(LabProg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ImGui Fuentes (Mismos que en el .py)
set(IMGUI_SOURCES
    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)

set(SOURCES
    src/main.cpp
    src/ShaderProgram.cpp
    src/InputManager.cpp
    src/DebugManager.cpp
    src/Camera.cpp
    src/Triangle.cpp
    lib/glad/src/gl.c
    ${IMGUI_SOURCES}
)

# Directorios de inclusión para AMBOS (Win/Linux)
include_directories(
    lib/glad/include
    lib/glfw/include
    lib/glm
    lib/imgui
    lib/imgui/misc
    lib/imgui/backends
    lib/json
    lib/stb
    lib/tinygltf
    lib/tinyobj
)

add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32 AND MINGW)
    target_link_libraries(${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/lib/glfw/src/libglfw3.a
        opengl32 gdi32 user32 kernel32
        -static-libstdc++ -static-libgcc
    )
else()
    # En Linux, usamos las librerías del sistema
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    target_link_libraries(${PROJECT_NAME}
        glfw
        ${OPENGL_LIBRARIES}
        X11 pthread dl
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)