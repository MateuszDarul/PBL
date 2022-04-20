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

    PointLightComponent::needUpdate = true;
    PointLightComponent::lightAmount++;

    float step = 0.1;
    for(int i=0; i<10000; i++)
    {
        this->range += step;
        if( (1 / (this->distance.x + this->range * this->distance.y + this->range * this->range * this->distance.z)) < 0.01 )
            break;
    }

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