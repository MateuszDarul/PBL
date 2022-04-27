#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTex;

out vec2 vertexTexture;

uniform mat4 transform;
uniform mat4 model;

void main()
{
    gl_Position = transform * model * vec4(vPos, 1.0);
    vertexTexture = vTex;
}
