#!/usr/bin/env bash
set -euo pipefail

# Este script arranca Xvfb + x11vnc + websockify/noVNC y ejecuta el binario OpenGL.
# Úsalo en GitHub Codespaces o cualquier Linux headless.

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

export PATH="$HOME/.local/bin:$PATH"

XVFB_DISPLAY=":1"
RESOLUTION="1280x720x24"
VNC_PORT=5900
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

if ! command -v websockify >/dev/null || ! python3 -c "import websockify" >/dev/null 2>&1; then
  python3 -m pip install --user websockify
  export PATH="$HOME/.local/bin:$PATH"
fi

# noVNC (clonamos si no exista)
if [ ! -d "$ROOT/VNC/noVNC" ]; then
  echo "2) Clonando noVNC..."
  git clone https://github.com/novnc/noVNC.git "$ROOT/VNC/noVNC"
fi

if [ -f "$ROOT/VNC/noVNC/utils/novnc_proxy" ]; then
  chmod +x "$ROOT/VNC/noVNC/utils/novnc_proxy"
fi

# Arrancamos Xvfb en segundo plano
if pgrep -f "Xvfb $XVFB_DISPLAY" >/dev/null; then
  echo "Xvfb ya estaba corriendo en $XVFB_DISPLAY"
else
  echo "3) Arrancando Xvfb en $XVFB_DISPLAY ($RESOLUTION)"
  Xvfb $XVFB_DISPLAY -screen 0 $RESOLUTION &
  sleep 2
fi

# Arrancamos x11vnc
if pgrep -f "x11vnc .* -rfbport $VNC_PORT" >/dev/null; then
  echo "x11vnc ya está corriendo en puerto $VNC_PORT"
else
  echo "4) Arrancando x11vnc en $VNC_PORT con input habilitado"
  x11vnc -display $XVFB_DISPLAY -forever -shared -nopw -rfbport $VNC_PORT -grabkbd -grabptr -xkb -ncache 10 -ncache_cr &
  sleep 1
  if ss -ltnp 2>/dev/null | grep -q ":$VNC_PORT"; then
    echo "x11vnc escuchando en el puerto $VNC_PORT"
  else
    echo "Advertencia: x11vnc no parece estar escuchando en $VNC_PORT" >&2
  fi
fi

# Arrancamos noVNC proxy
if pgrep -f "novnc_proxy.*$WEB_PORT" >/dev/null || pgrep -f "websockify.*$WEB_PORT" >/dev/null; then
  echo "noVNC/websockify ya está corriendo en puerto $WEB_PORT"
else
  echo "5) Arrancando noVNC proxy en $WEB_PORT -> localhost:$VNC_PORT"
  if [ -x "$ROOT/VNC/noVNC/utils/novnc_proxy" ]; then
    "$ROOT/VNC/noVNC/utils/novnc_proxy" --listen "$WEB_PORT" --vnc "localhost:$VNC_PORT" --web "$ROOT/VNC/noVNC" &
  else
    websockify --web "$ROOT/VNC/noVNC" $WEB_PORT localhost:$VNC_PORT &
  fi
  sleep 1
  if ss -ltnp 2>/dev/null | grep -q ":$WEB_PORT"; then
    echo "websockify/noVNC proxy escuchando en el puerto $WEB_PORT"
  else
    echo "Advertencia: websockify/noVNC proxy no parece estar escuchando en $WEB_PORT" >&2
  fi
fi

# Ejecutar la aplicación
if [ -x "$ROOT/bin/LabProg" ]; then
  echo "6) Ejecutando bin/LabProg en DISPLAY=$XVFB_DISPLAY"
  DISPLAY=$XVFB_DISPLAY "$ROOT/bin/LabProg" &
else
  echo "6) No se encontró ejecutable. Compilando automáticamente..."
  mkdir -p build
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build -- -j$(nproc)

  if [ -x "$ROOT/bin/LabProg" ]; then
    echo "7) Ejecutando bin/LabProg en DISPLAY=$XVFB_DISPLAY"
    DISPLAY=$XVFB_DISPLAY "$ROOT/bin/LabProg" &
  else
    echo "Error: Falló la compilación. Revisa logs arriba." >&2
    exit 1
  fi
fi

echo "Listo: abre en tu navegador el web port forwarding del Codespace:"
echo "  http://localhost:$WEB_PORT/vnc.html?host=localhost&port=$WEB_PORT"
echo "Para input: haz clic en la ventana para dar foco, luego usa teclado/mouse."
echo "También puedes exponer el puerto 6080 en el panel de Ports de Codespaces."
