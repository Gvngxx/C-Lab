# TODOs

## Hecho

- [x] Crear la ventana y el contexto OpenGL con GLFW y GLAD.
- [x] Cargar y compilar shaders desde `assets/shaders/`.
- [x] Cargar texturas y dibujar modelos y geometría básica.
- [x] Importar modelos mediante Assimp.
- [x] Añadir cámara controlable, entrada de teclado/ratón y controles alternativos para VNC.
- [x] Añadir una implementación inicial del jugador con gravedad y métodos de movimiento/salto.
- [x] Añadir herramientas de depuración con Dear ImGui.
- [x] Configurar la ejecución headless con Xvfb, VNC y noVNC.
- [x] Tener el despliegue web del proyecto mediante VNC/noVNC en Codespaces.
- [x] Añadir un núcleo de simulación Microverse independiente del renderizado.
- [x] Añadir absorción, transferencia, manipulación, barrera física y mantenimiento de maldición.
- [x] Mostrar estado de energía y controles de prueba en el panel ImGui.
- [x] Añadir tests autónomos del núcleo sin modificar el target de CMake existente.
- [x] Documentar build y ejecución en Windows 11 con el MinGW incluido.

## Pendiente

- [x] Instalar dependencias de sistema y verificar compilación y ejecución de la ventana en Linux con Xvfb/VNC/noVNC.
- [ ] Verificar compilación y ejecución de la ventana en Windows.
- [ ] Implementar reproducción y gestión de audio con OpenAL.
- [ ] Sustituir los cubos de prueba por renderizado visual de energía/materiales.
- [ ] Extender el prototipo a unidades de energía, interacciones espaciales escalables y transformaciones configurables.
- [ ] Añadir entidades/componentes generales, bendiciones/catálogo extensible de maldiciones y guardado persistente.
- [ ] Añadir un registro de eventos acumulado; ahora solo se muestra el último evento.
- [ ] Integrar los tests con CTest sin cambiar el flujo habitual de compilación.