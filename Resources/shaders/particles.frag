#version 450 core

in vec2 vertexTexture;

out vec4 FragColor;

uniform sampler2D textureData;

void main()
{
    vec4 frag = texture(textureData, vertexTexture);
    if(frag.a < 0.5)
	discard;
    FragColor = frag;
}
