# VNC + noVNC para C-Lab en Codespaces

Este directorio contiene scripts para ejecutar la app OpenGL dentro de un servidor X virtual y acceder con navegador usando noVNC.

## Requisitos
- Linux (GitHub Codespaces / container)
- `Xvfb`, `x11vnc`, `python3-pip`, `git`
- opcional: `wine64` (para ejecutar `bin/CppLabExample.exe` si es un exe de Windows)

## Uso rápido
1. Compila tu proyecto y genera `bin/CppLabExample.exe` (o `bin/CppLabExample`).
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
- En PC Windows, ejecuta usualmente `bin/CppLabExample.exe` directamente.
- En Codespaces Linux se usa `wine` si existe el `.exe`, o el binario Linux si está presente.
