#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTex;
layout (location = 3) in mat4 mInstanceMatrix;

uniform bool inst;

uniform mat4 model;

void main()
{
    if(inst)
    {
        gl_Position = model * mInstanceMatrix * vec4(vPos, 1.0);
    }
    else
    {
        gl_Position = model * vec4(vPos, 1.0);
    }
}