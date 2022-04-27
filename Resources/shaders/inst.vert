#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in mat4 mInstanceMatrix;

out vec2 texCoords;

uniform mat4 transform;
uniform mat4 model;

void main()
{
    texCoords = vTexCoords;
    gl_Position = transform * model * mInstanceMatrix * vec4(vPos, 1.0);
}