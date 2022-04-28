#include "PointLightComponent.h"

#include "GameObject.h"

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
    if(this->GetOwner()->GetComponent<TransformComponent>() == nullptr)
    {
        this->transform = std::make_shared<TransformComponent>();
        this->GetOwner()->AddComponent(transform);
    }

    this->wasCreated = true;
    this->transform->SetPosition(glm::vec3(0,0,0));
    this->lightColor = glm::vec3(0.5,0.5,0.5);
    this->specularColor = glm::vec3(1,1,1);
    this->distance = glm::vec3(1.f,0.25f,0.f);

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

void PointLightComponent::Use(std::shared_ptr<ShaderComponent> shader)
{
    if(!PointLightComponent::needUpdate || !this->wasCreated)
    {
        return;
    }

    textID = std::to_string(PointLightComponent::thisLightID);
    shader->SetInt("pointLightAmount", PointLightComponent::lightAmount);
    shader->SetVec3("pointLight[" + textID + "].position", this->transform->GetPosition());
    shader->SetVec3("pointLight[" + textID + "].lightColor", this->lightColor);
    shader->SetVec3("pointLight[" + textID + "].specularColor", this->specularColor);
    shader->SetVec3("pointLight[" + textID + "].distanceVec", this->distance);

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