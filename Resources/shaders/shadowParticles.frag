#version 450 core

struct PointLight
{
    vec3 position;
	
    float distance;
};

struct SpotLight 
{
    vec3 position; 
    vec3 direction;

    float cutOff;
    float outerCutOff;
	
    float distance;
};

in vec2 vertexTexture;
in vec3 fragPos;
in float lifetimePercentage;

out vec4 FragColor;

uniform sampler2D particleTexture;

uniform vec4 u_ColorStart;
uniform vec4 u_ColorEnd;

uniform vec3 cameraPos;

uniform int pointLightAmount;
uniform PointLight pointLight[10];

uniform int spotLightAmount;
uniform SpotLight spotLight[10];

float GetPointLight(PointLight pointLight);
float GetSpotLight(SpotLight sLight);

void main()
{
    vec4 frag = texture(particleTexture, vertexTexture);

    float t = min(1.0, max(0.0, lifetimePercentage));

    vec4 col = u_ColorStart * (1.0 - t) + u_ColorEnd * t;
    
    frag.rgb = frag.rgb * col.rgb;
    frag.a *= col.a;
	
    if(frag.a < 0.05)
	discard;

    if(pointLightAmount == 0 && spotLightAmount == 0)
    {
        discard;
    }
    else
    {
	float light = 0;
        for(int i=0; i<spotLightAmount; i++)
        {
            float intensity = GetSpotLight(spotLight[i]);
	        if(intensity >= 0.1f && intensity > light)
                light = intensity;        
        }
	    if(light < 0.1f)
	        discard;
    }
    FragColor = frag;
}


float GetSpotLight(SpotLight sLight)
{

    vec3 lightDir = normalize(sLight.position - fragPos);
       
    float theta = dot(lightDir, normalize(-sLight.direction)); 
    float epsilon = (sLight.cutOff - sLight.outerCutOff);
    float intensity = clamp((theta - sLight.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(sLight.position - fragPos);
    float attenuation = 1 - pow(distance * sLight.distance, 5.0);

    intensity *= attenuation;

    return intensity;
}