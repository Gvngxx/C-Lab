#version 430 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out vec3 Normal;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPos, 1.0);

    TexCoord = texCoord;

    Normal = mat3(transpose(inverse(model))) * vertexNormal;
}