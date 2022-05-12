#version 450 core

in float gradient;
out vec4 FragColor;

uniform vec3 u_Color1;
uniform vec3 u_Color2;

void main()
{    
    vec3 col = mix(u_Color1, u_Color2, gradient);
    FragColor = vec4(col, 1.0);
}  