#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;
layout (location = 2) in mat4 mInstanceMatrix;

out vec2 vertexTexture;
out float lifetimePercentage;

uniform mat4 matrixPV;
uniform mat4 model;
uniform mat4 cameraView;
uniform float width;
uniform float height;

void main()
{
    vec4 trans = {0,0,0,1};
    mat4 newModel = inverse(cameraView);
    newModel[3] = trans;

    lifetimePercentage = mInstanceMatrix[0][3];
    
    mat4 correctedInstanceMatrix = mInstanceMatrix;
    correctedInstanceMatrix[0][3] = 0.0;

    gl_Position = matrixPV * model * correctedInstanceMatrix * newModel * vec4(vPos,1.0f);
    
    vertexTexture = vTex;
}
