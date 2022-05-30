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
}
