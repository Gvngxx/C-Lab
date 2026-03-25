# C++ Lab Example

Este es un ejemplo simple de un proyecto C++ que utiliza CMake y MinGW64 para compilar una aplicación OpenGL con GLFW y GLAD.

## Uso Rápido

1. **Doble clic en `build_and_run.bat`** o ejecuta `.\build_and_run.ps1`
2. El script compilará todo automáticamente y ejecutará el programa
3. Verás una ventana azul de OpenGL que se cierra al presionar ESC

## Estructura del Proyecto

- **src/**: Contiene los archivos fuente del proyecto (main.cpp).
- **lib/**: Contiene las bibliotecas externas necesarias:
  - **glad/**: Para cargar funciones de OpenGL.
  - **glfw/**: Para crear ventanas y manejar entrada.
  - **glm/**: Biblioteca de matemáticas para gráficos (no usada en este ejemplo simple).
  - **imgui/**: Interfaz de usuario inmediata (no usada en este ejemplo).
  - **json/**: Biblioteca para JSON (no usada).
  - **stb/**: Bibliotecas de imágenes y utilidades (no usadas).
  - **tinygltf/**: Para cargar modelos glTF (no usado).
  - **tinyobj/**: Para cargar objetos OBJ (no usado).
- **bin/**: Directorio donde se genera el ejecutable compilado.
- **assets/**: Para recursos como modelos, texturas, etc. (vacío en este ejemplo).
- **mingw64/**: Contiene el compilador MinGW64 y herramientas.
- **build_and_run.bat**: Script batch para compilar y ejecutar automáticamente.
- **build_and_run.ps1**: Script PowerShell para compilar y ejecutar automáticamente.
- **compile.txt**: Contiene comandos de compilación manual.

## Compilación

### Opción 1: Scripts Automáticos (Recomendado)

**Windows Batch (.bat):**
```cmd
build_and_run.bat
```

**PowerShell (.ps1):**
```powershell
.\build_and_run.ps1
```

Estos scripts compilan GLFW automáticamente si no existe, compilan el proyecto y lo ejecutan, mostrando todos los logs.

### Opción 2: Usando CMake (Manual)

1. Crear directorio de build:

```bash
mkdir build
cd build
```

1. Configurar con CMake:

```bash
cmake ..
```

1. Compilar:

```bash
make
```

El ejecutable se generará en `bin/`.

### Opción 3: Compilación Manual (Ver compile.txt)

Para compilar manualmente con g++, ver el archivo `compile.txt`.

## Explicación del Código

El ejemplo crea una ventana simple usando GLFW y OpenGL. Muestra una pantalla azul y se cierra al presionar ESC o cerrar la ventana.

- **GLFW**: Maneja la creación de ventanas, eventos de entrada y contexto OpenGL.
- **GLAD**: Carga las funciones de OpenGL dinámicamente.
- **OpenGL**: API gráfica para renderizar.

## Notas

- Los scripts `build_and_run.bat` y `build_and_run.ps1` manejan automáticamente la compilación de GLFW si no existe
- Para PowerShell, es posible que necesites cambiar la política de ejecución: `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser`
- El proyecto usa OpenGL 3.3 Core Profile
- La ventana se puede cerrar presionando ESC o cerrando la ventana normalmente
