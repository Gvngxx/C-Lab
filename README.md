# C-Lab

Proyecto de motor 3D en C++17. Actualmente crea una ventana OpenGL, dibuja modelos y texturas, permite controlar la cámara y contiene una implementación básica de jugador. OpenAL Soft está integrado como dependencia de compilación; todavía no hay reproducción de audio en el código.

## Requisitos

- CMake 3.13 o posterior, compilador C/C++ y Git.
- Bibliotecas de desarrollo de OpenGL y X11 en Linux.
- Los submódulos Git de Assimp, noVNC y OpenAL Soft.
- En Windows 11 se puede usar el toolchain MinGW incluido en `mingw64/`.

Tras clonar el repositorio, descarga los submódulos:

```bash
git submodule update --init --recursive
```

## Compilar y ejecutar

Desde la raíz del proyecto:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./bin/LabProg
```

La aplicación carga sus shaders y recursos desde `assets/`, así que ejecútala desde la raíz del repositorio.

## Windows 11

Abre PowerShell en la raíz del repositorio. El toolchain incluido debe contener CMake, GCC, G++ y MinGW Make en `mingw64/bin/`; inicializa primero los submódulos:

```powershell
git submodule update --init --recursive
$cmake = ".\mingw64\bin\cmake.exe"
$gcc = "$($PWD.Path)\mingw64\bin\gcc.exe"
$gxx = "$($PWD.Path)\mingw64\bin\g++.exe"
$make = "$($PWD.Path)\mingw64\bin\mingw32-make.exe"
& $cmake -S . -B build -G "MinGW Makefiles" `
	"-DCMAKE_C_COMPILER=$gcc" `
	"-DCMAKE_CXX_COMPILER=$gxx" `
	"-DCMAKE_MAKE_PROGRAM=$make" `
	-DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cmake --build build --parallel 4
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
.\bin\LabProg.exe
```

Ejecuta desde la raíz para que la aplicación encuentre `assets/`. Estos comandos llaman a CMake directamente y no incrementan `assets/version.txt`; el script `build.py` sí cambia ese archivo antes de compilar.

## Ejecutar en Codespaces con VNC/noVNC

Este es el flujo principal para desplegar la ventana del proyecto desde la web. Instala las dependencias del sistema indicadas en [VNC/README.md](VNC/README.md), compila el proyecto y ejecuta:

```bash
./VNC/start_vnc.sh
```

Expón el puerto `6080` en Codespaces y abre la URL de noVNC que muestra el script. Haz clic en la ventana para enviarle teclado y ratón. La aplicación se ve desde el navegador mediante VNC, no con una ejecución nativa local del escritorio.

> Si quieres lanzar la ventana en un entorno local o Windows, usa la sección de build de Windows 11 que aparece arriba; la web/remote del proyecto se despliega con VNC/noVNC.

## Estructura

- `src/`: aplicación, cámara, entrada, jugador, gráficos y depuración.
- `assets/`: modelos, texturas y shaders utilizados por la aplicación.
- `third_party/`: dependencias gráficas y utilidades; Assimp y OpenAL Soft son submódulos.
- `VNC/`: scripts y configuración para ejecutar la aplicación en un entorno Linux sin pantalla.
- `TODOs.md`: funciones implementadas y trabajo pendiente.

## Simulación Microverse (prototipo)

La simulación inicial vive en `src/simulation/` y funciona sin depender de GLFW/OpenGL. Sus valores editables están en `assets/config/microverse.json`. El loop actual de la aplicación la avanza a 60 Hz y el panel existente de ImGui muestra el estado real.

Controles: `1` activa/desactiva Tenkaikai, `2` Mugen no Me, `E` transfiere energía almacenada, `R` manipula la fuente bendita, `B` intenta formar una barrera, `O` lanza el objeto de prueba y `F` inspecciona el objeto maldito. Los mismos comandos están disponibles en el panel de depuración.

Los tests del núcleo son autónomos y no cambian el target CMake de la aplicación:

```bash
c++ -std=c++17 -Wall -Wextra -Wpedantic -Isrc -Ithird_party/json \
	src/simulation/Simulation.cpp tests/simulation_tests.cpp -o /tmp/microverse_simulation_tests
/tmp/microverse_simulation_tests
```

En PowerShell, usando el compilador incluido:

```powershell
& .\mingw64\bin\g++.exe -std=c++17 -Wall -Wextra -Wpedantic -Isrc -Ithird_party/json `
	src/simulation/Simulation.cpp tests/simulation_tests.cpp -o build\microverse_simulation_tests.exe
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& .\build\microverse_simulation_tests.exe
```

La simulación ya cubre absorción limitada por tasa/capacidad, separación de energías, transferencia, validación de alcance, coste de manipulación, barrera con cohesión/estabilidad, colisión del objeto de prueba y mantenimiento/fallback de una maldición. La configuración JSON conserva valores abstractos, no unidades físicas validadas.

La ventana gráfica no se pudo compilar ni ejecutar en el contenedor de desarrollo: faltan los encabezados de desarrollo XRandR y OpenGL del sistema. En Linux, instala primero los paquetes enumerados en [VNC/README.md](VNC/README.md); la ejecución gráfica en Windows 11 aún debe verificarse en una máquina Windows.