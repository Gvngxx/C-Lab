# Script PowerShell para compilar y ejecutar el proyecto C++
Write-Host "====================================================================" -ForegroundColor Cyan
Write-Host "                COMPILANDO Y EJECUTANDO PROYECTO C++" -ForegroundColor Cyan
Write-Host "====================================================================" -ForegroundColor Cyan

# Verificar si GLFW esta compilado
$glfwLib = "lib\glfw\build\src\libglfw3.a"
if (-not (Test-Path $glfwLib)) {
    Write-Host "GLFW no encontrado. Compilando GLFW..." -ForegroundColor Yellow
    Push-Location "lib\glfw"
    New-Item -ItemType Directory -Force -Path "build" | Out-Null
    Set-Location "build"

    Write-Host "Configurando GLFW con CMake..." -ForegroundColor Green
    $cmakeCmd = "..\..\..\mingw64\bin\cmake.exe .. -G `"MinGW Makefiles`" -DCMAKE_C_COMPILER=C:\workspace\C++LAB\mingw64\bin\gcc.exe -DCMAKE_CXX_COMPILER=C:\workspace\C++LAB\mingw64\bin\g++.exe -DCMAKE_MAKE_PROGRAM=C:\workspace\C++LAB\mingw64\bin\mingw32-make.exe -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5"
    Invoke-Expression $cmakeCmd

    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Fallo en configuracion de GLFW" -ForegroundColor Red
        Read-Host "Presiona Enter para salir"
        exit 1
    }

    Write-Host "Compilando GLFW..." -ForegroundColor Green
    $makeCmd = "..\..\..\mingw64\bin\mingw32-make.exe"
    Invoke-Expression $makeCmd

    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Fallo en compilacion de GLFW" -ForegroundColor Red
        Read-Host "Presiona Enter para salir"
        exit 1
    }

    Pop-Location
    Write-Host "GLFW compilado exitosamente." -ForegroundColor Green
} else {
    Write-Host "GLFW ya esta compilado." -ForegroundColor Green
}

# Eliminar ejecutable anterior
Write-Host "Eliminando ejecutable anterior..." -ForegroundColor Yellow
Remove-Item "bin\CppLabExample.exe" -ErrorAction SilentlyContinue

# Compilar proyecto principal
Write-Host "Compilando proyecto principal..." -ForegroundColor Green
$gppCmd = "mingw64\bin\g++.exe -o bin\CppLabExample.exe src\main.cpp lib\glad\src\gl.c -I lib\glad\include -I lib\glfw\include -L lib\glfw\build\src -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -std=c++17"
Invoke-Expression $gppCmd

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Fallo en compilacion del proyecto" -ForegroundColor Red
    Read-Host "Presiona Enter para salir"
    exit 1
}

# Ejecutar programa
Write-Host "Compilacion exitosa. Ejecutando programa..." -ForegroundColor Green
Write-Host "====================================================================" -ForegroundColor Cyan
$exeCmd = "bin\CppLabExample.exe"
Invoke-Expression $exeCmd

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: El programa termino con codigo $LASTEXITCODE" -ForegroundColor Red
}

Write-Host "====================================================================" -ForegroundColor Cyan
Write-Host "Proceso completado." -ForegroundColor Green
Read-Host "Presiona Enter para salir"