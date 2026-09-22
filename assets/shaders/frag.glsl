#version 430 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D texture1;
uniform float Time;

void main()
{
    vec3 normal = normalize(Normal);

    vec3 lightDirection = normalize(vec3(1.0, 2.0, 3.0));

    float light = max(dot(normal, lightDirection), 0.0);

    light = 0.35 + light * 0.65;

    vec4 texColor = texture(texture1, TexCoord);

    FragColor = vec4(texColor.rgb * light, texColor.a);
}