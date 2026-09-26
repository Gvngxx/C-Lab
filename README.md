# CPP Lab

Un motor de juegos 3D moderno escrito en C++ que utiliza OpenGL, GLFW y GLAD para renderizado en tiempo real. Diseñado para desarrollo profesional de juegos y aplicaciones gráficas.

## Uso Rápido

1. Compila el proyecto con CMake usando `python build.py` o desde el directorio raíz con:
   ```bash
   mkdir -p build && cmake -S . -B build && cmake --build build -- -j4
   ```
2. Ejecuta `bin/LabProg`.
3. Si usas Codespaces/VNC, inicia `./VNC/start_vnc.sh` y accede al cliente noVNC en el puerto `6080`. Para input, haz clic en la ventana para dar foco.

## Estructura del Proyecto

- **src/**: Código fuente del motor organizado en módulos:
  - **core/**: Punto de entrada principal (main.cpp).
  - **graphics/**: Renderizado y gráficos (ShaderProgram, Triangle, Camera).
  - **input/**: Gestión de entrada (InputManager, Keyboard).
  - **debug/**: Herramientas de debugging (DebugManager).
- **third_party/**: Bibliotecas externas:
  - **glad/**: Loader de funciones OpenGL.
  - **glfw/**: Gestión de ventanas y entrada (del sistema).
  - **glm/**: Matemáticas 3D.
  - **imgui/**: Interfaz de usuario para debugging.
  - **json/**: Parsing JSON.
  - **stb/**: Utilidades de imágenes.
  - **OpenAl/**: Libreria para el audio.
  - **Assimp/**: Carga de modelos 3D.
- **bin/**: Ejecutables compilados.
- **VNC/**: Configuración para ejecución en entornos headless.
- **assets/**: Recursos del juego (modelos, texturas, shaders).
- **build/**: Archivos de compilación CMake.

## Compilación

### Compilación con CMake (Recomendada)

1. Crear el directorio de build y generar la configuración:

```bash
mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

2. Compilar el proyecto:

```bash
cmake --build build -- -j4
```

3. Ejecutar el binario:

```bash
./bin/LabProg
```

### Uso en VNC / Codespaces

1. Compila el proyecto antes de iniciar VNC.
2. Ejecuta el script VNC:

```bash
./VNC/start_vnc.sh
```

3. Exponer el puerto `6080` y abre en el navegador:

```bash
http://localhost:6080/vnc.html?host=localhost&port=6080
```

### Opción alternativa: script Python

También puedes usar el script `python build.py`, que ahora invoca CMake para configurar, compilar y ejecutar el proyecto.

## Explicación del Código

El ejemplo crea una ventana simple usando GLFW y OpenGL. Muestra una pantalla azul y se cierra al presionar ESC o cerrar la ventana.

- **GLFW**: Maneja la creación de ventanas, eventos de entrada y contexto OpenGL.
- **GLAD**: Carga las funciones de OpenGL dinámicamente.
- **OpenGL**: API gráfica para renderizar.