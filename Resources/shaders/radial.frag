#version 450 core

in vec2 vertexTexture;
in vec3 VertexPosition;

out vec4 FragColor;

uniform sampler2D diffuseMapData;
uniform sampler2D specularMapData;
uniform sampler2D normalMapData;

uniform vec4 u_TintColor; //HACK - using rgba channels not for color: r = progress, g = secondary size, b = hollow factor, a = 

uniform float brightness;
uniform float gamma;
uniform float contrast;

///--------------------------------------------------------- CODE

vec4 bright(vec4 colorIn, float brightnessVal);
vec4 gam(vec4 colorIn, float gammaVal);
vec4 cont(vec4 colorIn, float contrastVal);

void main()
{
    if (length(VertexPosition) < u_TintColor.b) discard;

    float o = 0.0;

    vec3 point = normalize(VertexPosition);
    // float angle = dot(vec3(1.0, 0.0, 0.0), point) * 0.5 + 0.5;
    float angle = acos(dot(vec3(-1.0, 0.0, 0.0), point)) * 0.31830989 * (1.0 + 2.0*o) - o; // 1 / pi

    if (angle < 0.0 || angle > 1.0) discard;

    vec4 mainColor = { 1.0, 1.0, 1.0,  1.0 };
    vec4 backColor = { 0.5, 0.5, 0.5,  0.5 };
    vec4 secondaryColor = { 1.0, 0.0, 0.0,  1.0 };
    
    if (angle > u_TintColor.r)
        FragColor = texture(diffuseMapData, vertexTexture) * backColor;
    else if (angle > u_TintColor.r - u_TintColor.g)
        FragColor = texture(diffuseMapData, vertexTexture) * secondaryColor;
    else
        FragColor = texture(diffuseMapData, vertexTexture) * mainColor;

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