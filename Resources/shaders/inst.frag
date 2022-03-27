#version 450 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

void main()
{
    FragColor = vec4(1.f, 0.f, 0.f, 1.f);
}