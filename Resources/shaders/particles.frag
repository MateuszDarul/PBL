#version 450 core

in vec2 vertexTexture;
in float lifetimePercentage;

out vec4 FragColor;

uniform sampler2D particleTexture;

uniform vec4 u_ColorStart;
uniform vec4 u_ColorEnd;

uniform float brightness;
uniform float gamma;
uniform float contrast;

///--------------------------------------------------------- CODE

vec4 bright(vec4 colorIn, float brightnessVal);
vec4 gam(vec4 colorIn, float gammaVal);
vec4 cont(vec4 colorIn, float contrastVal);

void main()
{
    vec4 frag = texture(particleTexture, vertexTexture);

    float t = min(1.0, max(0.0, lifetimePercentage));

    vec4 col = u_ColorStart * (1.0 - t) + u_ColorEnd * t;
    
    frag.rgb = frag.rgb * col.rgb;
    frag.a *= col.a;

    if(frag.a < 0.05)
	    discard;
    FragColor = frag;

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