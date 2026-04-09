#version 430 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform float Time;

void main(){
    FragColor = texture(texture1, TexCoord);
}