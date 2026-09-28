#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LOG_DIR="$ROOT/VNC/terminal"

stop_matching() {
  local label="$1"
  local pattern="$2"
  local pids

  pids="$(pgrep -f -- "$pattern" || true)"
  if [[ -z "$pids" ]]; then
    printf '%s is not running.\n' "$label"
    return
  fi

  while IFS= read -r pid; do
    [[ -n "$pid" && "$pid" != "$$" && "$pid" != "$PPID" ]] || continue
    if kill -TERM "$pid" 2>/dev/null; then
      printf 'Stopped %s (PID %s).\n' "$label" "$pid"
    fi
  done <<< "$pids"
}

stop_matching "LabProg" "^$ROOT/bin/LabProg$"
stop_matching "noVNC/websockify on port 6080" 'novnc_proxy.*--listen 6080|websockify.*6080.*localhost:5900'
stop_matching "x11vnc on port 5900" '^x11vnc .* -rfbport 5900( |$)'
stop_matching "Xvfb display :1" '^Xvfb :1( |$)'
stop_matching "LabProg log web server on port 6081" "^python3 -m http.server 6081 --bind 0.0.0.0 --directory $LOG_DIR$"

printf 'VNC services stopped. The log file was kept at %s/LabProg.log.\n' "$LOG_DIR"