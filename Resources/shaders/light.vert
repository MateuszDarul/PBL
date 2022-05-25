#version 450 core

///--------------------------------------------------------- LAYOUT

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTex;
layout (location = 3) in mat4 mInstanceMatrix;

///--------------------------------------------------------- OUT

out vec3 fragPos;
out vec2 vertexTexture;
out vec3 normalVEC;
out mat4 modelTransformations;

///--------------------------------------------------------- UNIFORM

uniform bool inst;
uniform mat4 transform;
uniform mat4 model;

///--------------------------------------------------------- CODE

void main()
{
    if(inst)
    {
        mat4 fullTransformMatrix = model * mInstanceMatrix;

        fragPos = vec3(fullTransformMatrix * vec4(vPos, 1.0));
        vertexTexture = vTex;
        normalVEC = vNor;
        modelTransformations = fullTransformMatrix;

        gl_Position = transform * fullTransformMatrix * vec4(vPos, 1.0);
    }
    else
    {    
        fragPos = vec3(model * vec4(vPos, 1.0));
        vertexTexture = vTex;
        normalVEC = vNor;
        modelTransformations = model;

        gl_Position = transform * model * vec4(vPos, 1.0);
    }
}
