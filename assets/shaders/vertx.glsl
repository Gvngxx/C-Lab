#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model; // Si luego quieres mover el triángulo por separado

void main() {
    // El orden de multiplicación es CRÍTICO: Proyección * Vista * Posición
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}