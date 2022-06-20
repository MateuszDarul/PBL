#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTex;

out vec2 TexCoord;
out vec3 Normal;
out float Noise;
out vec3 VertexPos;
out vec3 FragmentPos;

uniform mat4 transform;
uniform mat4 model;
uniform float u_Time;

uniform sampler2D specularMapData;


mat2 rotMat(float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c, -s, s, c);
}

void main()
{
 
    TexCoord = vTex;
    mat3 fixnormal = mat3(model[0][0], model[0][1], model[0][2], model[1][0], model[1][1], model[1][2], model[2][0], model[2][1], model[2][2]);
    Normal = fixnormal * vNor;
    

    /* ===== TEXTURES

    diffuse:
     rgba - main color

    bump (normal):
     rgba - fresnel color

    specular:
     r - 'electricity' highlight on/off
     g - vertex displacement noise (main shape)
     b - unused ? (gives almost unnoticeably small effect)
     a - highlight noise

    ==================*/

    vec4 pnoise = texture(specularMapData, rotMat(u_Time) * Normal.xy + vPos.y * vPos.z);
    Noise = pnoise.g;

    vec3 d = Normal * (pnoise.g * 2.0 - 1.0) * 0.2 ;//+ Normal * pnoise.b * 0.05;
    vec3 newPos = vPos + d;
    VertexPos = newPos;

    vec4 worldSpacePos = model * vec4(newPos, 1.0);
    FragmentPos = worldSpacePos.xyz;

    gl_Position = transform * worldSpacePos;

}
