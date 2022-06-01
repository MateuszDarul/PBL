#include "PointLightComponent.h"

#include "GameObject.h"

int PointLightComponent::lightAmount = 0;
int PointLightComponent::thisLightID = 0;
bool PointLightComponent::needUpdate = false;

PointLightComponent::PointLightComponent()
    :LightComponent(8)
{
    this->wasCreated = false;
}

PointLightComponent::~PointLightComponent()
{
    this->Destroy();
}

bool PointLightComponent::Create()
{
    if(this->GetOwner()->GetComponent<TransformComponent>() == nullptr)
    {
        this->transform = std::make_shared<TransformComponent>();
        this->GetOwner()->AddComponent(transform);
    }

    this->wasCreated = true;
    this->transform->SetPosition(glm::vec3(0,0,0));
    this->lightColor = glm::vec3(1,1,1);
    this->specularColor = glm::vec3(1,1,1);
    this->damping = 10;

    PointLightComponent::needUpdate = true;
    PointLightComponent::lightAmount++;

    return true;
}

bool PointLightComponent::Destroy()
{
    if(this->wasCreated)
    {
        PointLightComponent::needUpdate = true;
        PointLightComponent::lightAmount--;
        this->wasCreated = false;
        this->transform = nullptr;
        return true;
    }
    return false;
}

void PointLightComponent::Use()
{
    if(!PointLightComponent::needUpdate || !this->wasCreated)
    {
        return;
    }

    textID = std::to_string(PointLightComponent::thisLightID);
    for(uint16_t i=0; i<shaders.size(); i++)
    {
        shaders[i]->Use();
        shaders[i]->SetInt("pointLightAmount", PointLightComponent::lightAmount);
        shaders[i]->SetVec3("pointLight[" + textID + "].position", this->transform->GetPosition());
        shaders[i]->SetVec3("pointLight[" + textID + "].lightColor", this->lightColor);
        shaders[i]->SetVec3("pointLight[" + textID + "].specularColor", this->specularColor);
        shaders[i]->SetFloat("pointLight[" + textID + "].distance", this->damping * DAMPING_MUL);
    }

    if(PointLightComponent::thisLightID == PointLightComponent::lightAmount - 1)
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
    this->transform->SetPosition(position);
    PointLightComponent::needUpdate = true;
}

void PointLightComponent::Move(glm::vec3 vector)
{
    this->transform->Move(vector);
    PointLightComponent::needUpdate = true;
}

glm::vec3 PointLightComponent::GetPosition()
{
    return this->transform->GetPosition();
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

void PointLightComponent::SetDamping(float damping)
{
    this->damping = damping;
    PointLightComponent::needUpdate = true;
}

float PointLightComponent::GetDamping()
{
    return this->damping;
}