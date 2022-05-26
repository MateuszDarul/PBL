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


float random( vec3 scale, float seed )
{
  return fract( sin( dot( gl_FragCoord.xyz + seed, scale ) ) * 43758.5453 + seed ) ;
}

void main()
{
   
    vec4 pnoise = texture(specularMapData, TexCoord - vec2(0.0, u_Time));
    vec4 aa = texture(specularMapData, TexCoord + vec2(-u_Time, u_Time));

    float var = texture(specularMapData, vec2(VertexPos.z, VertexPos.x)).a;
    float highlight = texture(specularMapData, vec2(0.5, 0.5)).r * smoothstep(0.88, 1.0, fract(var + cos(VertexPos.z * 5.78) * sin(VertexPos.x * 2.78) * 0.66 + VertexPos.y + u_Time) * 2.0 - 1.0);
    
    float x = -1.11 * u_Time;
    float fac = 3.141592;
    float blink = min(min(1.0, 13.0 * fac * fract(x)), min(1.0, 17.0 * fac * fract(0.5 * (x - 0.4))));
    blink = 1.0 - blink;
    //highlight += (1.0 - texture(specularMapData, vec2(0.5, 0.5)).r) * blink * smoothstep(0.92, 1.0, fract(var + cos(VertexPos.z * 0.86) * sin(VertexPos.y * 2.78) * 0.66 + VertexPos.x + 16.0*u_Time) * 2.0 - 1.0);//smoothstep(0.88, 0.9, fract(-var + cos(VertexPos.z * 0.78) * sin(VertexPos.x * 0.332) * 0.76 + 16.0*u_Time) * 2.0 - 1.0);
    //highlight = blink * smoothstep(0.92, 1.0, fract(var + cos(VertexPos.z * 0.86) * sin(VertexPos.y * 2.78) * 0.66 + VertexPos.x + 16.0*u_Time) * 2.0 - 1.0);
    highlight = aa.r *(highlight + blink * smoothstep(0.92, 1.0, fract(var + cos(VertexPos.z * 0.86) * sin(VertexPos.y * 2.78) * 0.66 + VertexPos.x + 16.0*u_Time) * 2.0 - 1.0));

    vec4 color = texture(diffuseMapData, vec2(0.0, 0.0));
    color.rgb += highlight;


    float alphafactor = smoothstep(0.99, 0.97, color.a);
    float ao = alphafactor + (1 - alphafactor) * max(0.5, Noise);//smoothstep(0.9, 0.7, Noise);


    //FRESNEL
    vec3 lookDir = normalize(FragmentPos - u_CameraPos);
    float fresnel = dot(Normal, -lookDir);
    fresnel = clamp(1 - fresnel, 0.0, 1.0);

    vec4 fresnelCol = texture(normalMapData, vec2(0.0, 0.0));
    color = fresnel * fresnelCol + (1.0-fresnel)*color;
    //color.rgb = fresnel * fresnelCol + 1-color.rgb;


    // FragColor = vec4(abs(TexCoord.x * 2.0 - 1.0), TexCoord.y, 0.0, 1.0);
    // FragColor = vec4(pnoise.rr, aa.b, 1.0);
    // FragColor = vec4(color, ao * 0.7);
    FragColor = vec4(color.rgb * ao, color.a);
}
