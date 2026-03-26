@echo off
echo ====================================================================
echo                COMPILANDO Y EJECUTANDO PROYECTO C++
echo ====================================================================

echo Verificando si GLFW esta compilado...
if not exist "lib\glfw\build\src\libglfw3.a" (
    echo GLFW no encontrado. Compilando GLFW...
    cd lib\glfw
    mkdir build 2>nul
    cd build

    echo Configurando GLFW con CMake...
    ..\..\..\mingw64\bin\cmake.exe .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER=C:\workspace\C++LAB\mingw64\bin\gcc.exe -DCMAKE_CXX_COMPILER=C:\workspace\C++LAB\mingw64\bin\g++.exe -DCMAKE_MAKE_PROGRAM=C:\workspace\C++LAB\mingw64\bin\mingw32-make.exe -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5

    if %errorlevel% neq 0 (
        echo ERROR: Fallo en configuracion de GLFW
        pause
        exit /b 1
    )

    echo Compilando GLFW...
    ..\..\..\mingw64\bin\mingw32-make.exe

    if %errorlevel% neq 0 (
        echo ERROR: Fallo en compilacion de GLFW
        pause
        exit /b 1
    )

    cd ..\..\..
    echo GLFW compilado exitosamente.
) else (
    echo GLFW ya esta compilado.
)

echo Eliminando ejecutable anterior...
del /Q bin\CppLabExample.exe 2>nul

echo Compilando proyecto principal...
mingw64\bin\g++.exe -o bin\CppLabExample.exe src\main.cpp src\shaderprogram.cpp src\triangle.cpp  lib\glad\src\gl.c -I lib\glad\include -I lib\glfw\include -L lib\glfw\build\src -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -std=c++17

if %errorlevel% neq 0 (
    echo ERROR: Fallo en compilacion del proyecto
    pause
    exit /b 1
)

echo Compilacion exitosa. Ejecutando programa...
echo ====================================================================
bin\CppLabExample.exe

if %errorlevel% neq 0 (
    echo ERROR: El programa termino con codigo %errorlevel%
)

echo ====================================================================
echo Proceso completado.
pause