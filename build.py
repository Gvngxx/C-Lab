import os
import subprocess
import sys
import time
import threading
from colorama import init, Fore, Style

# Inicializa colores
init(autoreset=True)

# --- CONFIGURACIÓN DE RUTAS ---
BASE_DIR = os.getcwd()
MINGW_PATH = os.path.join(BASE_DIR, "mingw64", "bin")
GXX = os.path.join(MINGW_PATH, "g++.exe")
CMAKE = os.path.join(MINGW_PATH, "cmake.exe")
MAKE = os.path.join(MINGW_PATH, "mingw32-make.exe")

# Variable de control para la animación
compilando = False

def animacion(mensaje):
    puntos = ["   ", ".  ", ".. ", "..."]
    idx = 0
    while compilando:
        sys.stdout.write(f"\r{Fore.CYAN}{mensaje}{puntos[idx % 4]}")
        sys.stdout.flush()
        idx += 1
        time.sleep(0.4)
    sys.stdout.write(f"\r{Fore.CYAN}{mensaje}... {Fore.GREEN}[OK]\n")

def ejecutar_con_animacion(comando, mensaje, error_msg, cwd=None):
    global compilando
    compilando = True

    # Iniciamos el hilo de la animacion
    t = threading.Thread(target=animacion, args=(mensaje,))
    t.start()
    
    # Ejecutamos el proceso real
    resultado = subprocess.run(comando, shell=True, capture_output=True, text=True, cwd=cwd)
    
    # Detenemos la animacion
    compilando = False
    t.join()
    
    if resultado.returncode != 0:
        print(f"\n{Fore.RED}{Style.BRIGHT}ERROR: {error_msg}")
        print(f"{Fore.YELLOW}DETALLES:\n{resultado.stderr}")
        input("\nPresiona ENTER para salir...")
        sys.exit(1)

def build():
    os.system('cls' if os.name == 'nt' else 'clear')
    time.sleep(1)
    print(f"{Fore.MAGENTA}{Style.BRIGHT}====================================================")
    print(f"{Fore.WHITE}{Style.BRIGHT}            NORAD BUILD SYSTEM | 2026 EDITION       ")
    print(f"{Fore.MAGENTA}{Style.BRIGHT}====================================================\n")

    # 1. GESTION DE GLFW
    glfw_lib_path = os.path.join(BASE_DIR, "lib", "glfw", "build", "src", "libglfw3.a")
    
    if not os.path.exists(glfw_lib_path):
        build_dir = os.path.join(BASE_DIR, "lib", "glfw", "build")
        if not os.path.exists(build_dir): os.makedirs(build_dir)

        cmake_cmd = (
            f'"{CMAKE}" .. -G "MinGW Makefiles" '
            f'-DCMAKE_C_COMPILER="{os.path.join(MINGW_PATH, "gcc.exe")}" '
            f'-DCMAKE_CXX_COMPILER="{GXX}" '
            f'-DCMAKE_MAKE_PROGRAM="{MAKE}" '
            f'-DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF'
        )
        ejecutar_con_animacion(cmake_cmd, "[INFO] Configurando GLFW", "Fallo en CMake.", cwd=build_dir)
        ejecutar_con_animacion(f'"{MAKE}"', "[INFO] Compilando librerias GLFW", "Fallo en Make.", cwd=build_dir)
    else:
        print(f"{Fore.YELLOW}[SKIP] GLFW ya está listo.")

    # 2. LIMPIEZA
    if os.path.exists(r"bin\LabProg.exe"):
        os.remove(r"bin\LabProg.exe")

    # 3. COMPILACION PRINCIPAL
    sources = [
        r"src\main.cpp", r"src\ShaderProgram.cpp", r"src\InputManager.cpp",
        r"src\DebugManager.cpp", r"src\Triangle.cpp", r"src\Camera.cpp", r"lib\glad\src\gl.c",
        r"lib\imgui\imgui.cpp", r"lib\imgui\imgui_draw.cpp", r"lib\imgui\imgui_demo.cpp",
        r"lib\imgui\imgui_tables.cpp", r"lib\imgui\imgui_widgets.cpp",
        r"lib\imgui\backends\imgui_impl_glfw.cpp", r"lib\imgui\backends\imgui_impl_opengl3.cpp"
    ]

    flags = [
        f'-o "bin\LabProg.exe"',
        f'-I"lib\imgui"',
        f'-I"lib\imgui\backends"',
        f'-I"lib\glad\include"', 
        f'-I"lib\glfw\include"',
        f'-I"lib\glm"',
        f'"{glfw_lib_path}"',
        "-lopengl32 -lgdi32 -luser32 -lkernel32",
        "-std=c++17", "-static-libgcc -static-libstdc++"
    ]

    cmd_final = f'"{GXX}" {" ".join(sources)} {" ".join(flags)}'
    
    ejecutar_con_animacion(cmd_final, "[INFO] Compilando Proyecto Principal", "Fallo al compilar el proyecto.")

    # 4. FINALIZACIÓN Y EJECUCIÓN
    print(f"\n{Fore.MAGENTA}{Style.BRIGHT}====================================================")
    print(f"{Fore.GREEN}{Style.BRIGHT}--- COMPILACION EXITOSA ---")
    print(f"{Fore.CYAN}[EXEC] Iniciando programa")
    print(f"{Fore.MAGENTA}{Style.BRIGHT}====================================================\n")
    time.sleep(1)

    subprocess.run(r"bin\LabProg.exe", shell=True)

    print(f"\n{Fore.MAGENTA}{Style.BRIGHT}====================================================\n")

if __name__ == "__main__":
    build()