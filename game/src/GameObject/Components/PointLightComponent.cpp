#include "PointLightComponent.h"

int PointLightComponent::lightAmount = 0;
int PointLightComponent::thisLightID = 0;
bool PointLightComponent::needUpdate = false;

PointLightComponent::PointLightComponent()
    :Component(8)
{
    this->wasCreated = false;
}

PointLightComponent::~PointLightComponent()
{
    this->Destroy();
}

bool PointLightComponent::Create()
{
    this->wasCreated = true;
    this->position = glm::vec3(0,0,0);
    this->lightColor = glm::vec3(0.5,0.5,0.5);
    this->specularColor = glm::vec3(1,1,1);
    this->distance = glm::vec3(1.f,0.25f,0.f);
    this->range = 0;

    PointLightComponent::needUpdate = true;
    PointLightComponent::lightAmount++;

    float step = 0.1;
    for(int i=0; i<10000; i++)
    {
        this->range += step;
        if( 
            (1 / (this->distance.x + this->range * this->distance.y + this->range * this->range * this->distance.z))
             < 0.01
        )
            break;
    }
    std::cout << range << "\n";

    return true;
}

bool PointLightComponent::Destroy()
{
    if(this->wasCreated)
    {
        PointLightComponent::needUpdate = true;
        PointLightComponent::lightAmount--;
        this->wasCreated = false;
        return true;
    }
    return false;
}

void PointLightComponent::Use(std::shared_ptr<ShaderComponent> shader)
{
    if(!PointLightComponent::needUpdate || !this->wasCreated)
    {
        return;
    }


    textID = std::to_string(PointLightComponent::thisLightID);
    shader->SetInt("lightAmount", PointLightComponent::lightAmount);
    shader->SetVec3("pointLight[" + textID + "].position", this->position);
    shader->SetVec3("pointLight[" + textID + "].lightColor", this->lightColor);
    shader->SetVec3("pointLight[" + textID + "].specularColor", this->specularColor);
    shader->SetVec3("pointLight[" + textID + "].distanceVec", this->distance);
    shader->SetFloat("pointLight[" + textID + "].range", this->range);


    if(PointLightComponent::thisLightID == PointLightComponent::lightAmount)
    {
        PointLightComponent::thisLightID = 0;
        PointLightComponent::needUpdate = false;
    }
    else
    {
        PointLightComponent::thisLightID++;
    }
}

void PointLightComponent::SetPosition(glm::vec3 position)
{
    this->position = position;
    PointLightComponent::needUpdate = true;
}

void PointLightComponent::Move(glm::vec3 vector)
{
    this->position += vector;
    PointLightComponent::needUpdate = true;
}

glm::vec3 PointLightComponent::GetPosition()
{
    return this->position;
}

void PointLightComponent::SetLightColor(glm::vec3 lightColor)
{
    this->lightColor = lightColor;
    PointLightComponent::needUpdate = true;
}

glm::vec3 PointLightComponent::GetLightColor()
{
    return this->lightColor;
}

void PointLightComponent::SetSpecularColor(glm::vec3 specularColor)
{
    this->specularColor = specularColor;
    PointLightComponent::needUpdate = true;
}

glm::vec3 PointLightComponent::GetSpecularColor()
{
    return this->specularColor;
}

void PointLightComponent::SetRange(float constant, float linear, float quadratic)
{
    this->distance.x = constant;
    this->distance.y = linear;
    this->distance.z = quadratic;
    PointLightComponent::needUpdate = true;
}

glm::vec3 PointLightComponent::GetRange()
{
    return this->distance;
}