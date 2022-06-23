#version 450 core

in vec3 Normal;
in vec3 Position;

out vec4 FragColor;

uniform float ratio;
uniform vec3 u_CameraPos;
uniform samplerCube skybox;

uniform float brightness;
uniform float gamma;
uniform float contrast;

///--------------------------------------------------------- CODE

vec4 bright(vec4 colorIn, float brightnessVal);
vec4 gam(vec4 colorIn, float gammaVal);
vec4 cont(vec4 colorIn, float contrastVal);

void main()
{
    vec3 I = normalize(Position - u_CameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);

    FragColor = bright(FragColor, brightness);
    FragColor = cont(FragColor, contrast);
    FragColor = gam(FragColor, gamma);
}

vec4 bright(vec4 colorIn, float brightnessVal)
{
    return vec4(colorIn.x * brightnessVal, colorIn.y * brightnessVal, colorIn.z * brightnessVal, colorIn.a);
}

vec4 gam(vec4 colorIn, float gammaVal)
{
    return vec4(pow(colorIn.x, gammaVal), pow(colorIn.y, gammaVal), pow(colorIn.z, gammaVal), colorIn.a);
}

vec4 cont(vec4 colorIn, float contrastVal)
{
    float t = 0.5f - contrastVal * 0.5;
    return vec4(colorIn.x * t, colorIn.y * t, colorIn.z * t, colorIn.a);
}