#version 450 core

in vec4 vertex_color;
in vec2 vertex_texture;

out vec4 FragColor;

uniform sampler2D data_texture;
uniform bool use_texture;

void main()
{
    if(use_texture)
    {
        FragColor = texture(data_texture, vertex_texture);
    }
    else
    {
        FragColor = vertex_color;
    }
}
