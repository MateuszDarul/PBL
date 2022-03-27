#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in mat4 mInstanceMatrix;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} vs_out;

uniform mat4 transform;

void main()
{
    vs_out.fragPos = vec3(mInstanceMatrix * vec4(vPos, 1.0));
    vs_out.normal = mat3(transpose(inverse(mInstanceMatrix))) * vNormal;

    vs_out.texCoords = vTexCoords;
    gl_Position = transform * mInstanceMatrix * vec4(vPos, 1.0);
}