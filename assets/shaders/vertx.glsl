#version 430 core

layout(location = 0) in vec3 aPos;

uniform float Time;

void main() { 

    gl_Position = vec4(aPos.x + Time, aPos.y, aPos.z, 1.0);
}