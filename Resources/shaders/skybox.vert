#version 450 core

layout (location = 0) in vec3 vPos;

out vec3 vertexTexture;

uniform mat4 transform;

void main()
{
    vertexTexture = vPos;
    vec4 pos = transform * vec4(vPos, 1.0);
    gl_Position = pos.xyww;
}
