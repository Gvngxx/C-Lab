
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
del /Q bin\LabProg.exe 2>nul

echo Compilando proyecto principal...
mingw64\bin\g++.exe -o bin\LabProg.exe ^
src\main.cpp ^
src\shaderprogram.cpp ^
src\InputManager.cpp ^
src\DebugManager.cpp ^
src\Camera.cpp ^
src\triangle.cpp ^
lib\glad\src\gl.c ^
lib\imgui\imgui.cpp ^
lib\imgui\imgui_draw.cpp ^
lib\imgui\imgui_demo.cpp ^
lib\imgui\imgui_tables.cpp ^
lib\imgui\imgui_widgets.cpp ^
lib\imgui\backends\imgui_impl_glfw.cpp ^
lib\imgui\backends\imgui_impl_opengl3.cpp ^
-I lib\imgui ^
-I lib\imgui\backends ^
-I lib\glad\include ^
-I lib\glfw\include ^
-L lib\glfw\build\src ^
-lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 ^
-std=c++17

if %errorlevel% neq 0 (
    echo ERROR: Fallo en compilacion del proyecto
    pause
    exit /b 1
)

echo Compilacion exitosa
timeout 5 > NUL

echo Ejecutando programa...
echo ====================================================================
bin\LabProg.exe

if %errorlevel% neq 0 (
    echo ERROR: El programa termino con codigo %errorlevel%
)

echo ====================================================================
echo Proceso completado.
exit /b 0
