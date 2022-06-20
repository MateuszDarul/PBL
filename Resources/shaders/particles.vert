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
    lifetimePercentage = mInstanceMatrix[0][3];
    
    mat4 newModel = inverse(cameraView);
    newModel[3] = mInstanceMatrix[3] + model[3];
    newModel[3][3] = 1.0;

    float scl = mInstanceMatrix[0][0];
    float t = lifetimePercentage * 0.04;
    float s = sin(t);
    float c = cos(t) * scl;
    
    mat4 rotnscale  = mat4(c, s, 0.0, 0.0,  -s, c, 0.0, 0.0,  0.0, 0.0, scl, 0.0,  0.0, 0.0, 0.0, 1.0); // Z-azis

    gl_Position = matrixPV * newModel * rotnscale * vec4(vPos,1.0f);
    
    vertexTexture = vTex;
}
