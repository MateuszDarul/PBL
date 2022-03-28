#version 450 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D diffuseMapData;
uniform sampler2D specularMapData;
uniform sampler2D normalMapData;

void main()
{
    FragColor = texture(diffuseMapData, texCoords);
}