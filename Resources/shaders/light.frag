#version 450 core

///--------------------------------------------------------- STRUCTURS

struct PointLight
{
    vec3 position;

    vec3 lightColor;
    vec3 specularColor;
	
    vec3 distanceVec;
    // distanceVec.x = constant;
    // distanceVec.y = linear;
    // distanceVec.z = quadratic;
};

struct SpotLight 
{
    vec3 position; 
    vec3 direction;

    vec3 lightColor;
    vec3 specularColor;

    float cutOff;
    float outerCutOff;
	
    vec3 distanceVec;
    // distanceVec.x = constant;
    // distanceVec.y = linear;
    // distanceVec.z = quadratic;
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

///--------------------------------------------------------- CODE

vec3 GetPointLight(PointLight pointLight);

void main()
{
    vec3 pixelColor = vec3(0,0,0);

    if(pointLightAmount == 0)
    {
        pixelColor = texture(diffuseMapData, vertexTexture).rgb;
    }
    else
    {
        for(int i=0; i<pointLightAmount; i++)
        {
            pixelColor += GetPointLight(pointLight[i]);
        }
    }

    FragColor = vec4(pixelColor, 1.0f);
}

vec3 GetPointLight(PointLight pLight)
{
    vec3 colorMAP = texture(diffuseMapData, vertexTexture).rgb;
    vec3 specularMAP = texture(specularMapData, vertexTexture).rgb;
    //vec3 normalMAP = texture(normalMapData, vertexTexture).xyz;
    vec3 normalMAP = normalVEC;
    normalMAP = normalize(mat3(transpose(inverse(model_transformations))) * normalMAP);

    vec3 viewDir = normalize(cameraPos - fragPos);

    vec3 ambient = 0.2 * colorMAP;

    vec3 lightDir = normalize(pLight.position - fragPos);
    float diff = max(dot(normalMAP, lightDir), 0.0);
    vec3 diffuse = pLight.lightColor * diff * colorMAP;

    vec3 reflectDir = reflect(-lightDir, normalMAP);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = pLight.specularColor * spec * specularMAP;

    float distance = length(pLight.position - fragPos);
    float attenuation;
    if(true)
    {
        /*
        //pLight.distanceVec = vec3(1.f,0.0f,0.f);
        attenuation = 1.0 / (
            pLight.distanceVec.x + 
            pLight.distanceVec.y * distance +
            pLight.distanceVec.z * distance * distance);
        */
        attenuation = 1 - 0.04 * distance;
    }
    else
    {
        if(distance < 16)
            attenuation = 1;
        else 
            attenuation = 0;
    }

    ambient *= attenuation;  
    diffuse *= attenuation;
    specular *= attenuation;   

    return ambient + diffuse + specular;
}
