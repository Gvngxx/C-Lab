cmake_minimum_required(VERSION 3.10)
project(CppLabExample)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configurar el compilador para MinGW
set(CMAKE_CXX_COMPILER "C:/workspace/C++LAB/mingw64/bin/g++.exe")
set(CMAKE_C_COMPILER "C:/workspace/C++LAB/mingw64/bin/gcc.exe")

# Incluir directorios de encabezados
include_directories(
    lib/glad/include
    lib/glfw/include
    lib/glm
    lib/imgui
    lib/json
    lib/stb
    lib/tinygltf
    lib/tinyobj
)

# Agregar archivos fuente
set(SOURCES
    src/main.cpp
    src/shaderprogram.cpp
    lib/glad/src/gl.c
)

# Crear ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Enlazar bibliotecas
target_link_libraries(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/lib/glfw/src/libglfw3.a
    opengl32
    gdi32
    user32
    kernel32
    -static-libstdc++ -static-libgcc
)

# Configurar directorio de salida
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)