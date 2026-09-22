# Limpiar procesos anteriores
echo "[-] Limpiando procesos anteriores..."
pkill -f "Xvfb $XVFB_DISPLAY" || true
pkill -f "x11vnc.*$VNC_PORT" || true
pkill -f "websockify.*$WEB_PORT" || true
sleep 1