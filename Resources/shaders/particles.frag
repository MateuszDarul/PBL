#version 450 core

in vec2 vertexTexture;

out vec4 FragColor;

uniform sampler2D texture;

void main()
{
    vec4 frag = texture(texture, vertexTexture);
    if(frag.a < 0.5)
	discard;
    FragColor = frag;
}
