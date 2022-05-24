#version 450 core

///--------------------------------------------------------- STRUCTURS

struct TextureMaps
{
    vec3 colorMAP;
    vec3 specularMAP;
    vec3 normalMAP;
};

struct PointLight
{
    vec3 position;

    vec3 lightColor;
    vec3 specularColor;
	
    float distance;
};

struct SpotLight 
{
    vec3 position; 
    vec3 direction;

    vec3 lightColor;
    vec3 specularColor;

    float cutOff;
    float outerCutOff;
	
    float distance;
};

///--------------------------------------------------------- IN

in vec2 vertexTexture;
in vec3 fragPos;
in vec3 normalVEC;
in mat4 model_transformations;

///--------------------------------------------------------- OUT

out vec4 FragColor;

///--------------------------------------------------------- UNIFORM

uniform sampler2D diffuseMapData;
uniform sampler2D specularMapData;
uniform sampler2D normalMapData;

uniform vec3 cameraPos;

uniform int pointLightAmount;
uniform PointLight pointLight[10];

uniform int spotLightAmount;
uniform SpotLight spotLight[10];

uniform float brightness;
uniform float gamma;
uniform float contrast;

///--------------------------------------------------------- CODE

vec3 GetPointLight(TextureMaps textureMaps, PointLight pointLight);
vec3 GetSpotLight(TextureMaps textureMaps, SpotLight sLight);

vec4 bright(vec4 colorIn, float brightnessVal);
vec4 gam(vec4 colorIn, float gammaVal);
vec4 cont(vec4 colorIn, float contrastVal);


void main()
{
    TextureMaps tm;
    tm.colorMAP = texture(diffuseMapData, vertexTexture).rgb;
    tm.specularMAP = texture(specularMapData, vertexTexture).rgb;
    tm.normalMAP = normalize(mat3(transpose(inverse(model_transformations))) * normalVEC);

    vec3 pixelColor = vec3(0,0,0);
    pixelColor += tm.colorMAP * 0.025;

    if(pointLightAmount == 0 && spotLightAmount == 0)
    {
        pixelColor = tm.colorMAP;
    }
    else
    {
        for(int i=0; i<pointLightAmount; i++)
        {
            pixelColor += GetPointLight(tm, pointLight[i]);
        }

        for(int i=0; i<spotLightAmount; i++)
        {
            pixelColor += GetSpotLight(tm, spotLight[i]);
        }
    }

    FragColor = vec4(pixelColor, 1.0f);

    FragColor = bright(FragColor, brightness);
    FragColor = cont(FragColor, contrast);
    FragColor = gam(FragColor, gamma);
}

vec3 GetPointLight(TextureMaps textureMaps, PointLight pLight)
{
    vec3 viewDir = normalize(cameraPos - fragPos);

    vec3 lightDir = normalize(pLight.position - fragPos);
    float diff = max(dot(textureMaps.normalMAP, lightDir), 0.0);
    vec3 diffuse = pLight.lightColor * diff * textureMaps.colorMAP;

    vec3 reflectDir = reflect(-lightDir, textureMaps.normalMAP);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = pLight.specularColor * spec * textureMaps.specularMAP;

    float distance = length(pLight.position - fragPos);
    float attenuation = 1 - pLight.distance * distance;

    diffuse *= attenuation;
    specular *= attenuation;   

    return diffuse + specular;
}

vec3 GetSpotLight(TextureMaps textureMaps, SpotLight sLight)
{
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);

    vec3 lightDir = normalize(sLight.position - fragPos);
    vec3 viewDir = normalize(cameraPos - fragPos);

    float diff = max(dot(textureMaps.normalMAP, lightDir), 0.0);
    diffuse = sLight.lightColor * diff * textureMaps.colorMAP;  
        
    vec3 reflectDir = reflect(-lightDir, textureMaps.normalMAP);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32/*material.shininess*/);
    specular = sLight.specularColor * spec * textureMaps.specularMAP;  
        
    float theta = dot(lightDir, normalize(-sLight.direction)); 
    float epsilon = (sLight.cutOff - sLight.outerCutOff);
    float intensity = clamp((theta - sLight.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    float distance = length(sLight.position - fragPos);
    float attenuation = 1 - sLight.distance * distance;

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
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