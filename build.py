import os
import subprocess
import sys
import time
import threading
from colorama import init, Fore, Style

# Inicializa colores
init(autoreset=True)

# ============================================================
# CONFIGURACIÓN
# ============================================================

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

MINGW_PATH = os.path.join(BASE_DIR, "mingw64", "bin")
GXX = os.path.join(MINGW_PATH, "g++.exe")
GCC = os.path.join(MINGW_PATH, "gcc.exe")
CMAKE = os.path.join(MINGW_PATH, "cmake.exe")
MAKE = os.path.join(MINGW_PATH, "mingw32-make.exe")

VERSION_FILE = os.path.join(BASE_DIR, "assets", "version.txt")
BUILD_DIR = os.path.join(BASE_DIR, "build")
BIN_DIR = os.path.join(BASE_DIR, "bin")
EXE_PATH = os.path.join(BIN_DIR, "LabProg") # Esto cambiara depende si es linux o windows

# Dependencias third_party que CMake compila dentro del build.
# OpenAL queda intencionalmente fuera.
REQUIRED_DIRS = [
    os.path.join(BASE_DIR, "third_party", "glfw"),
    os.path.join(BASE_DIR, "third_party", "assimp"),
    os.path.join(BASE_DIR, "third_party", "glad"),
    os.path.join(BASE_DIR, "third_party", "imgui"),
    os.path.join(BASE_DIR, "third_party", "glm"),
    os.path.join(BASE_DIR, "third_party", "json"),
    os.path.join(BASE_DIR, "third_party", "stb"),
]

compilando = False


# ============================================================
# UI
# ============================================================

def banner():
    print(f"{Fore.MAGENTA}{Style.BRIGHT}====================================================")
    print(f"{Fore.WHITE}{Style.BRIGHT}            NORAD BUILD SYSTEM | 2026 EDITION")
    print(f"{Fore.MAGENTA}{Style.BRIGHT}====================================================\n")


def section(title):
    print(f"\n{Fore.MAGENTA}{Style.BRIGHT}--- {title} ---")


def ok(message):
    print(f"{Fore.GREEN}[OK] {message}")


def info(message):
    print(f"{Fore.CYAN}[INFO] {message}")


def skip(message):
    print(f"{Fore.YELLOW}[SKIP] {message}")


def fail(message):
    print(f"{Fore.RED}{Style.BRIGHT}[ERROR] {message}")


# ============================================================
# ANIMACIÓN / PROCESOS
# ============================================================

def animacion(mensaje):
    puntos = ["   ", ".  ", ".. ", "..."]
    idx = 0

    while compilando:
        sys.stdout.write(
            f"\r{Fore.CYAN}{mensaje}{puntos[idx % 4]}"
        )
        sys.stdout.flush()
        idx += 1
        time.sleep(0.4)

    sys.stdout.write(
        f"\r{Fore.CYAN}{mensaje}... {Fore.GREEN}[OK]\n"
    )


def ejecutar_con_animacion(comando, mensaje, error_msg, cwd=None):
    global compilando

    compilando = True
    hilo = threading.Thread(
        target=animacion,
        args=(mensaje,),
        daemon=True
    )
    hilo.start()

    resultado = subprocess.run(
        comando,
        shell=True,
        capture_output=True,
        text=True,
        cwd=cwd
    )

    compilando = False
    hilo.join()

    if resultado.returncode != 0:
        fail(error_msg)

        if resultado.stderr.strip():
            print(f"{Fore.YELLOW}DETALLES:\n{resultado.stderr}")

        if resultado.stdout.strip():
            print(f"{Fore.YELLOW}SALIDA:\n{resultado.stdout}")

        input("\nPresiona ENTER para salir...")
        sys.exit(resultado.returncode)


def ejecutar_comando(comando, cwd=None):
    return subprocess.run(
        comando,
        shell=True,
        cwd=cwd
    ).returncode


# ============================================================
# VERSIONADO
# ============================================================

def leer_version():
    if not os.path.isfile(VERSION_FILE):
        fail(f"No existe: {VERSION_FILE}")
        sys.exit(1)

    with open(VERSION_FILE, "r", encoding="utf-8") as archivo:
        version = archivo.readline().strip()

    partes = version.split(".")

    if len(partes) != 3 or not all(parte.isdigit() for parte in partes):
        fail(
            "La versión debe tener formato MAJOR.MINOR.PATCH "
            f"(actual: '{version}')"
        )
        sys.exit(1)

    return version


def incrementar_patch():
    """
    Equivalente a:

    v=$(cat assets/version.txt)
    base=${v%.*}
    patch=${v##*.}
    echo "$base.$((patch + 1))" > assets/version.txt
    """

    version_anterior = leer_version()

    base, patch = version_anterior.rsplit(".", 1)
    nuevo_patch = int(patch) + 1
    nueva_version = f"{base}.{nuevo_patch}"

    with open(VERSION_FILE, "w", encoding="utf-8") as archivo:
        archivo.write(nueva_version + "\n")

    return version_anterior, nueva_version


# ============================================================
# VALIDACIÓN DEL ENTORNO
# ============================================================

def validar_herramientas():
    section("VERIFICACIÓN DEL ENTORNO")

    herramientas = {
        "G++": GXX,
        "GCC": GCC,
        "CMake": CMAKE,
        "MinGW Make": MAKE,
    }

    for nombre, ruta in herramientas.items():
        if not os.path.isfile(ruta):
            fail(f"{nombre} no encontrado: {ruta}")
            input("\nPresiona ENTER para salir...")
            sys.exit(1)

        ok(f"{nombre} encontrado")


def validar_third_party():
    section("VERIFICACIÓN DE BIBLIOTECAS")

    for ruta in REQUIRED_DIRS:
        nombre = os.path.basename(ruta)

        if os.path.isdir(ruta):
            ok(f"{nombre} disponible")
        else:
            fail(f"{nombre} no encontrado: {ruta}")
            input("\nPresiona ENTER para salir...")
            sys.exit(1)

    print()
    info("OpenAL: DESHABILITADO temporalmente")


# ============================================================
# CMAKE / DEPENDENCIAS
# ============================================================

def configurar_y_compilar_dependencias(clear_build=False):
    """
    Build normal:
        cmake --build build

    Build limpio:
        python3 build.py clear

    El modo 'clear' elimina build/, vuelve a configurar CMake desde cero
    con GCC/G++ en Release y después ejecuta make.
    """

    section("DEPENDENCIAS / CMAKE")

    if clear_build:
        print(
            f"{Fore.YELLOW}[CLEAR] Eliminando build anterior..."
        )

        if os.path.isdir(BUILD_DIR):
            import shutil
            shutil.rmtree(BUILD_DIR)

        ok("Directorio build eliminado.")

        os.makedirs(BUILD_DIR, exist_ok=True)

        # En Linux/Codespaces usamos gcc/g++ directamente.
        if os.name != "nt":
            configure_cmd = (
                f'cd "{BUILD_DIR}" && '
                f'cmake .. '
                f'-DCMAKE_C_COMPILER=gcc '
                f'-DCMAKE_CXX_COMPILER=g++ '
                f'-DCMAKE_BUILD_TYPE=Release'
            )

            ejecutar_con_animacion(
                configure_cmd,
                "Configurando CMake desde cero",
                "Falló la configuración limpia de CMake."
            )

            ejecutar_con_animacion(
                "make",
                "Compilando build limpio",
                "Falló make.",
                cwd=BUILD_DIR
            )

        else:
            # Fallback para Windows/MinGW.
            configure_cmd = (
                f'"{CMAKE}" -S "{BASE_DIR}" -B "{BUILD_DIR}" '
                f'-G "MinGW Makefiles" '
                f'-DCMAKE_C_COMPILER="{GCC}" '
                f'-DCMAKE_CXX_COMPILER="{GXX}" '
                f'-DCMAKE_MAKE_PROGRAM="{MAKE}" '
                f'-DCMAKE_BUILD_TYPE=Release '
                f'-DGLFW_BUILD_EXAMPLES=OFF '
                f'-DGLFW_BUILD_TESTS=OFF '
                f'-DGLFW_BUILD_DOCS=OFF '
                f'-DASSIMP_BUILD_TESTS=OFF '
                f'-DASSIMP_BUILD_ASSIMP_TOOLS=OFF '
                f'-DASSIMP_BUILD_SAMPLES=OFF '
                f'-DASSIMP_INSTALL=OFF'
            )

            ejecutar_con_animacion(
                configure_cmd,
                "Configurando CMake desde cero",
                "Falló la configuración limpia de CMake."
            )

            ejecutar_con_animacion(
                f'"{MAKE}"',
                "Compilando build limpio",
                "Falló MinGW Make."
                ,
                cwd=BUILD_DIR
            )

    else:
        # Build normal: NO borra build ni reconfigura CMake.
        # Simplemente construye usando la configuración existente.
        if not os.path.isdir(BUILD_DIR):
            fail(
                "No existe el directorio build. "
                "Ejecuta primero: python3 build.py clear"
            )
            input("\nPresiona ENTER para salir...")
            sys.exit(1)

        ejecutar_con_animacion(
            f'"{CMAKE}" --build "{BUILD_DIR}"',
            "Compilando build existente",
            "Falló cmake --build."
        )


# ============================================================
# BUILD PRINCIPAL
# ============================================================

def build(clear_build=False):
    os.system("cls" if os.name == "nt" else "clear")
    banner()

    validar_herramientas()
    validar_third_party()

    # --------------------------------------------------------
    # VERSION
    # --------------------------------------------------------
    section("VERSION")

    version_anterior, nueva_version = incrementar_patch()

    print(
        f"{Fore.YELLOW}Version anterior: "
        f"{Fore.WHITE}{version_anterior}"
    )
    print(
        f"{Fore.GREEN}Nueva versión:     "
        f"{Fore.WHITE}{Style.BRIGHT}{nueva_version}"
    )

    # --------------------------------------------------------
    # CMAKE / LIBS / MOTOR
    # --------------------------------------------------------
    configurar_y_compilar_dependencias(clear_build=clear_build)

    # --------------------------------------------------------
    # EJECUCIÓN
    # --------------------------------------------------------
    section("FINALIZACIÓN")

    if os.path.isfile(EXE_PATH):
        ok(f"Build generado: {EXE_PATH}")

        print(
            f"\n{Fore.MAGENTA}{Style.BRIGHT}"
            "===================================================="
        )
        print(
            f"{Fore.GREEN}{Style.BRIGHT}"
            "--- COMPILACIÓN EXITOSA ---"
        )
        print(
            f"{Fore.CYAN}[VERSION] {Fore.WHITE}{nueva_version}"
        )
        print(
            f"{Fore.CYAN}[EXEC] Iniciando programa"
        )
        print(
            f"{Fore.MAGENTA}{Style.BRIGHT}"
            "====================================================\n"
        )

        time.sleep(1)
        subprocess.run([EXE_PATH], shell=False)

    else:
        fail(
            "CMake termino, pero no se encontro "
            f"{EXE_PATH}"
        )
        input("\nPresiona ENTER para salir...")
        sys.exit(1)


if __name__ == "__main__":
    clear_build = len(sys.argv) > 1 and sys.argv[1].lower() == "clear"

    # Cualquier argumento distinto de "clear" se rechaza para evitar
    # ejecutar un modo inesperado.
    if len(sys.argv) > 1 and not clear_build:
        print(
            f"{Fore.YELLOW}Uso:"
            f"\n  python3 build.py"
            f"\n  python3 build.py clear"
        )
        sys.exit(1)

    build(clear_build=clear_build)
