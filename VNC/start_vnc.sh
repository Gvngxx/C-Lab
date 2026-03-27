#!/usr/bin/env bash
set -euo pipefail

# Este script arranca Xvfb + x11vnc + websockify/noVNC y ejecuta el binario OpenGL.
# Úsalo en GitHub Codespaces o cualquier Linux headless.

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

XVFB_DISPLAY=":1"
RESOLUTION="1280x720x24"
VNC_PORT=5901
WEB_PORT=6080

# Limpiar procesos anteriores
echo "0) Limpiando procesos anteriores..."
pkill -f "Xvfb $XVFB_DISPLAY" || true
pkill -f "x11vnc.*$VNC_PORT" || true
pkill -f "websockify.*$WEB_PORT" || true
sleep 1

echo "1) Instalar dependencias (requiere sudo/apt)"
if ! command -v Xvfb >/dev/null; then
  sudo apt-get update
  sudo apt-get install -y xvfb x11vnc python3-pip git
fi

if ! command -v websockify >/dev/null; then
  python3 -m pip install --user websockify
  export PATH="$HOME/.local/bin:$PATH"
fi

if ! command -v wine >/dev/null; then
  echo "Advertencia: wine no instalado. Instala con sudo apt-get install -y wine64." >&2
fi

# noVNC (clonamos si no existe)
if [ ! -d "$ROOT/VNC/noVNC" ]; then
  echo "2) Clonando noVNC..."
  git clone https://github.com/novnc/noVNC.git "$ROOT/VNC/noVNC"
fi

# Arrancamos Xvfb en segundo plano
if pgrep -f "Xvfb $XVFB_DISPLAY" >/dev/null; then
  echo "Xvfb ya estaba corriendo en $XVFB_DISPLAY"
else
  echo "3) Arrancando Xvfb en $XVFB_DISPLAY ($RESOLUTION)"
  Xvfb $XVFB_DISPLAY -screen 0 $RESOLUTION &
  sleep 2  # Esperar a que Xvfb esté listo
fi

# Arrancamos x11vnc
if pgrep -f "x11vnc .* -rfbport $VNC_PORT" >/dev/null; then
  echo "x11vnc ya está corriendo en puerto $VNC_PORT"
else
  echo "4) Arrancando x11vnc en $VNC_PORT"
  x11vnc -display $XVFB_DISPLAY -forever -shared -nopw -rfbport $VNC_PORT &
fi

# Arrancamos websockify (noVNC)
if pgrep -f "websockify $WEB_PORT" >/dev/null; then
  echo "websockify ya está corriendo en puerto $WEB_PORT"
else
  echo "5) Arrancando websockify en $WEB_PORT -> localhost:$VNC_PORT"
  websockify --web "$ROOT/VNC/noVNC" $WEB_PORT localhost:$VNC_PORT &
fi

# Ejecutar la aplicación
if [ -x "$ROOT/bin/LabProg.exe" ]; then
  echo "6) Ejecutando bin/LabProg.exe con wine en DISPLAY=$XVFB_DISPLAY"
  DISPLAY=$XVFB_DISPLAY wine "$ROOT/bin/LabProg.exe" &
elif [ -x "$ROOT/bin/LabProg" ]; then
  echo "6) Ejecutando bin/LabProg en DISPLAY=$XVFB_DISPLAY"
  DISPLAY=$XVFB_DISPLAY "$ROOT/bin/LabProg" &
else
  echo "6) No se encontró ejecutable. Compilando automáticamente..."
  # Compilar el proyecto
  echo "  Compilando proyecto..."
  mkdir -p build && cd build
  cmake .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
  make
  cd "$ROOT"
  # Ejecutar
  if [ -x "$ROOT/bin/LabProg" ]; then
    echo "7) Ejecutando bin/LabProg en DISPLAY=$XVFB_DISPLAY"
    DISPLAY=$XVFB_DISPLAY "$ROOT/bin/LabProg" &
  else
    echo "Error: Falló la compilación. Revisa logs arriba." >&2
  fi
fi

echo "Listo: abre en tu navegador el web port forwarding del Codespace:"
echo "  http://localhost:$WEB_PORT/vnc.html?host=localhost&port=$WEB_PORT"
echo "También puedes exponer el puerto 6080 en el panel de Ports de Codespaces."
