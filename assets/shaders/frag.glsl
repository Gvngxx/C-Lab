#version 430 core

out vec4 FragColor;

uniform float Time;

void main(){ 
    FragColor = vec4(-Time, -Time, Time, 1.0);
}