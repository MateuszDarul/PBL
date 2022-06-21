#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in float Noise;
in vec3 VertexPos;
in vec3 FragmentPos;

out vec4 FragColor;

uniform sampler2D diffuseMapData;
uniform sampler2D specularMapData;
uniform sampler2D normalMapData;

uniform float u_Time;
uniform vec3 u_CameraPos;

uniform float brightness;
uniform float gamma;
uniform float contrast;

///--------------------------------------------------------- CODE

vec4 bright(vec4 colorIn, float brightnessVal);
vec4 gam(vec4 colorIn, float gammaVal);
vec4 cont(vec4 colorIn, float contrastVal);

void main()
{
  //MAIN COLOR
  vec4 color = texture(diffuseMapData, TexCoord);


  //HIGHLIGHT
  vec4 hlNoise = texture(specularMapData, vec2(VertexPos.z, VertexPos.x));

  float highlight = smoothstep(0.88, 1.0, fract(hlNoise.a + cos(VertexPos.z * 5.78) * sin(VertexPos.x * 2.78) * 0.66 + sin(VertexPos.y) + u_Time) * 2.0 - 1.0);

  float x = -1.11 * u_Time;
  float fac = 3.141592;
  float blink = min(min(1.0, 13.0 * fac * fract(x)), min(1.0, 17.0 * fac * fract(0.5 * (x - 0.4))));
  blink = 1.0 - blink;
  highlight = hlNoise.r * (highlight + blink * smoothstep(0.92, 1.0, fract(hlNoise.a + cos(VertexPos.z * 0.86) * sin(VertexPos.y * 2.78) * 0.66 + VertexPos.x + 16.0*u_Time) * 2.0 - 1.0));
  
  color.rgb += highlight;


  //FRESNEL
  vec3 lookDir = normalize(FragmentPos - u_CameraPos);
  float fresnel = dot(Normal, -lookDir);
  fresnel = clamp(1 - fresnel, 0.0, 1.0);

  vec4 fresnelCol = texture(normalMapData, TexCoord);
  color = fresnel * fresnelCol + (1.0-fresnel)*color;


  //AMBIENT OCCLUSION-ish
  float alphafactor = smoothstep(0.99, 0.97, color.a);
  float ao = alphafactor + (1 - alphafactor) * max(0.5, Noise);


  //FINAL COLOR
  FragColor = vec4(color.rgb * ao, color.a);

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