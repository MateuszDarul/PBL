#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;
layout (location = 2) in mat4 mInstanceMatrix;

out vec2 vertexTexture;

uniform mat4 matrixPV;
uniform mat4 model;
uniform mat4 cameraView;
uniform float width;
uniform float height;

void main()
{
    vec4 trans = {1,1,1,1};
    mat4 newModel = inverse(cameraView);
    newModel[3] = trans;
    gl_Position = matrixPV * mInstanceMatrix * model * newModel * vec4(vPos,1.0f);
    vertexTexture = vTex;
}
