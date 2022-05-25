#version 450 core

in vec4 FragPos;

uniform vec3 lightPos;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    
    lightDistance = lightDistance * 0.01f;
    
    gl_FragDepth = lightDistance;
}