#version 450 core

layout (location = 0) in vec4 vertex;

uniform mat4 transform;
uniform mat4 model;

out float gradient;

void main()
{
    gl_Position = transform * model * vec4(vertex.xyz, 1.0);
    gradient = vertex.w;
} 