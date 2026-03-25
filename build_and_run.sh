#!/bin/bash

echo "===================================================================="
echo "                COMPILANDO Y EJECUTANDO PROYECTO C++"
echo "===================================================================="

echo "Verificando si GLFW está compilado..."
if [ ! -f "lib/glfw/build/src/libglfw3.a" ]; then
    echo "GLFW no encontrado. Compilando GLFW..."
    cd lib/glfw
    mkdir -p build
    cd build

    echo "Configurando GLFW con CMake..."
    cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF

    if [ $? -ne 0 ]; then
        echo "ERROR: Fallo en configuración de GLFW"
        exit 1
    fi

    echo "Compilando GLFW..."
    make

    if [ $? -ne 0 ]; then
        echo "ERROR: Fallo en compilación de GLFW"
        exit 1
    fi

    cd ../../..
fi

echo "Compilando el proyecto principal..."
echo "Eliminando ejecutable anterior..."
rm -f bin/CppLabExample

echo "Compilando con g++..."
g++ -o bin/CppLabExample src/*.cpp lib/glad/src/gl.c -I lib/glad/include -I lib/glfw/include -L lib/glfw/build/src -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -std=c++17

if [ $? -ne 0 ]; then
    echo "ERROR: Fallo en compilación del proyecto"
    exit 1
fi

echo "Verificando que se creó el ejecutable..."
if [ -f "bin/CppLabExample" ]; then
    echo "Ejecutable creado exitosamente."
    echo "Ejecutando..."
    ./bin/CppLabExample
else
    echo "ERROR: El ejecutable no se creó"
    exit 1
fi