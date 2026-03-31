# VNC + noVNC para C-Lab en Codespaces

Este directorio contiene scripts para ejecutar la app OpenGL dentro de un servidor X virtual y acceder con navegador usando noVNC.

## Requisitos
- Linux (GitHub Codespaces / container)
- `Xvfb`, `x11vnc`, `python3-pip`, `git`
<<<<<<< HEAD
- opcional: `wine64` (para ejecutar `bin/LagProg.exe` si es un exe de Windows)

## Uso rápido
1. Compila tu proyecto y genera `bin/LagProg.exe` (o `bin/LagProg`).
=======
- opcional: `wine64` (para ejecutar `bin/LabProg.exe` si es un exe de Windows)

## Uso rápido
1. Compila tu proyecto y genera `bin/LabProg.exe` (o `bin/LabProg`).
>>>>>>> 12eb20d406b4da5d6a8c16d02a66cb5ce93631ab
2. Desde el root de repositorio:
   ```bash
   ./VNC/start_vnc.sh
   ```
3. Exponer puerto 6080 en Codespaces (panel de Ports).
4. Abre:
   ```
   http://localhost:6080/vnc.html?host=localhost&port=6080
   ```

## Comportamiento
- Crea `Xvfb :1` y `x11vnc` sobre él.
- Mapea VNC a Websocket con `websockify`.
- Ejecuta tu binario en DISPLAY=:1.
- Verás la ventana de OpenGL dentro de noVNC.

## Nota de compatibilidad
<<<<<<< HEAD
- En PC Windows, ejecuta usualmente `bin/LagProg.exe` directamente.
=======
- En PC Windows, ejecuta usualmente `bin/LabProg.exe` directamente.
>>>>>>> 12eb20d406b4da5d6a8c16d02a66cb5ce93631ab
- En Codespaces Linux se usa `wine` si existe el `.exe`, o el binario Linux si está presente.
