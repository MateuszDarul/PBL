#version 450 core

///--------------------------------------------------------- STRUCTURES

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
in mat4 modelTransformations;

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

uniform bool depthMapStatus[10];
uniform vec3 depthMapPosition[10];
uniform samplerCube depthMap[10];



///--------------------------------------------------------- CODE

vec3 GetPointLight(TextureMaps textureMaps, PointLight pointLight);
vec3 GetSpotLight(TextureMaps textureMaps, SpotLight sLight);
float ShadowCalculation(int id, vec3 lightPos);

void main()
{
    TextureMaps tm;
    tm.colorMAP = texture(diffuseMapData, vertexTexture).rgb;
    tm.specularMAP = texture(specularMapData, vertexTexture).rgb;
    tm.normalMAP = normalize(mat3(transpose(inverse(modelTransformations))) * normalVEC);

    vec3 pixelColor = vec3(0,0,0);

    if(pointLightAmount == 0 && spotLightAmount == 0)
    {
        pixelColor = tm.colorMAP;
    }
    else
    {
        for(int i=0; i<pointLightAmount; i++)
        {
            vec3 col = GetPointLight(tm, pointLight[i]);
            if(col.x > 0.02 || col.y > 0.02 || col.z > 0.02)
                pixelColor += col;
        }

        for(int i=0; i<spotLightAmount; i++)
        {
            vec3 col = GetSpotLight(tm, spotLight[i]);
            if(col.x > 0.02 || col.y > 0.02 || col.z > 0.02)
                pixelColor += col;
        }

        float mul = 0;
        for(int i=0; i<10; i++)
        {
            if(depthMapStatus[i] == true)
            {
                mul += ShadowCalculation(i, depthMapPosition[i]);
            }
        }
        if(mul < 0.5)
        {
            pixelColor *= 0.25;
        }

    }

    pixelColor += tm.colorMAP * 0.07;
    
    FragColor = vec4(pixelColor, 1.0f);
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

float ShadowCalculation(int id, vec3 lightPos)
{
    vec3 fragToLight = fragPos - lightPos;

    float closestDepth = texture(depthMap[id], fragToLight).r;
    closestDepth *= 25;

    float currentDepth = length(fragToLight);
    float shadow = currentDepth - 0.1 > closestDepth ? 0.0 : 1.0;
    
    return shadow;
}