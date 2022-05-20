#version 450 core

///--------------------------------------------------------- LAYOUT

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTex;

///--------------------------------------------------------- OUT

out vec3 fragPos;
out vec2 vertexTexture;
out vec3 normalVEC;
out mat4 model_transformations;

///--------------------------------------------------------- UNIFORM

uniform mat4 transform;
uniform mat4 model;

///--------------------------------------------------------- CODE

void main()
{
    fragPos = vec3(model * vec4(vPos, 1.0));
    vertexTexture = vTex;
    normalVEC =  mat3(transpose(inverse(model))) * vNor;
    model_transformations = model;

    gl_Position = transform * model * vec4(vPos, 1.0);
}
