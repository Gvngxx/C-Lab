# VNC + noVNC para LVNG Engine en Codespaces

Este directorio contiene scripts para ejecutar el motor LVNG-Engine dentro de un servidor X virtual y acceder con un navegador usando noVNC.

## Requisitos
- Linux (GitHub Codespaces / container)
- `Xvfb`, `x11vnc`, `python3-pip`, `git`
- `pkg-config`, `libglfw3-dev`, `libgl1-mesa-dev`, `libglu1-mesa-dev`, `libx11-dev`, `libxrandr-dev`, `libxi-dev`, `libxcursor-dev`, `libxinerama-dev`, `libxkbcommon-dev`

## Uso rápido
1. Compila tu proyecto con CMake y genera `bin/LabProg`.
2. Desde la raíz del repositorio:
   ```bash
   ./VNC/start_vnc.sh
   ```
3. Exponer el puerto `6080` en Codespaces (panel de Ports).
4. Abre en tu navegador:
   ```
   http://localhost:6080/vnc.html?host=localhost&port=6080
   ```

> Nota: el servidor VNC interno usa `5900`, pero el acceso desde el navegador se hace a través del proxy noVNC en `6080`.

## Qué hace el script
- Crea un servidor X virtual con `Xvfb` en `:1`.
- Arranca `x11vnc` sobre ese display con captura de teclado y mouse (`-grabkbd -grabptr`).
- Arranca `websockify` para conectar noVNC al puerto web.
- Ejecuta `bin/LabProg` dentro de `DISPLAY=:1`.

## Notas
- En Linux/Desktop local, compile y ejecute `bin/LabProg` directamente.
- El script usa CMake para compilar automáticamente si no encuentra el ejecutable.
- Para input en noVNC: haga clic en la ventana para dar foco, luego use teclado/mouse normalmente.
