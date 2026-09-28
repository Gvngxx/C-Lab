# C-Lab

Proyecto de motor 3D en C++17. Actualmente crea una ventana OpenGL, dibuja modelos y texturas, permite controlar la cámara y contiene una implementación básica de jugador. OpenAL Soft está integrado como dependencia de compilación; todavía no hay reproducción de audio en el código.

## Requisitos

- CMake 3.13 o posterior, compilador C/C++ y Git.
- Bibliotecas de desarrollo de OpenGL y X11 en Linux.
- Los submódulos Git de Assimp, noVNC y OpenAL Soft.

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

## Ejecutar en Codespaces con VNC

Instala las dependencias del sistema indicadas en [VNC/README.md](VNC/README.md), compila el proyecto y ejecuta:

```bash
./VNC/start_vnc.sh
```

Expón el puerto `6080` en Codespaces y abre la URL de noVNC que muestra el script. Haz clic en la ventana para enviarle teclado y ratón.

## Estructura

- `src/`: aplicación, cámara, entrada, jugador, gráficos y depuración.
- `assets/`: modelos, texturas y shaders utilizados por la aplicación.
- `third_party/`: dependencias gráficas y utilidades; Assimp y OpenAL Soft son submódulos.
- `VNC/`: scripts y configuración para ejecutar la aplicación en un entorno Linux sin pantalla.
- `TODOs.md`: funciones implementadas y trabajo pendiente.