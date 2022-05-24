#version 450 core

in vec2 vertexTexture;

out vec4 FragColor;

uniform sampler2D diffuseMapData;
uniform sampler2D specularMapData;
uniform sampler2D normalMapData;

uniform vec4 u_TintColor;

void main()
{
    FragColor = texture(diffuseMapData, vertexTexture) * u_TintColor;
}
