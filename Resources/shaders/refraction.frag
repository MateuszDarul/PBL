#version 450 core

in vec3 Normal;
in vec3 Position;

out vec4 FragColor;

uniform float ratio;
uniform vec3 u_CameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(Position - u_CameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}