#include "SpotLightComponent.h"

#include "GameObject.h"

int SpotLightComponent::lightAmount = 0;
int SpotLightComponent::thisLightID = 0;
bool SpotLightComponent::needUpdate = false;

SpotLightComponent::SpotLightComponent()
    :Component(9)
{
    this->wasCreated = false;
}

SpotLightComponent::~SpotLightComponent()
{
    this->Destroy();
}

bool SpotLightComponent::Create()
{
    if(this->GetOwner()->GetComponent<TransformComponent>() == nullptr)
    {
        this->transform = std::make_shared<TransformComponent>();
        this->GetOwner()->AddComponent(transform);
    }

    this->wasCreated = true;
    this->transform->SetPosition(glm::vec3(0,0,0));
    this->transform->SetRotation(glm::vec3(0,0,0));
    this->lightColor = glm::vec3(0.5,0.5,0.5);
    this->specularColor = glm::vec3(1,1,1);
    this->distance = glm::vec3(1.f,0.25f,0.f);
    this->cutOff = glm::cos(glm::radians(12.5f));

    SpotLightComponent::needUpdate = true;
    SpotLightComponent::lightAmount++;

    return true;
}

bool SpotLightComponent::Destroy()
{
    if(this->wasCreated)
    {
        SpotLightComponent::needUpdate = true;
        SpotLightComponent::lightAmount--;
        this->wasCreated = false;
        this->transform = nullptr;
        return true;
    }
    return false;
}

void SpotLightComponent::SetPosition(glm::vec3 position)
{
    this->transform->SetPosition(position);
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::SetDirection(glm::vec3 direction)
{
    this->transform->SetRotation(direction);
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::SetLightColor(glm::vec3 lightColor)
{
    this->lightColor = lightColor;
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::SetSpecularColor(glm::vec3 specularColor)
{
    this->specularColor = specularColor;
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::SetRange(float constant, float linear, float quadratic)
{
    this->distance = glm::vec3(constant, linear, quadratic);
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::SetCutOff(float cutOff)
{
    this->cutOff = glm::cos(glm::radians(cutOff));
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::Move(glm::vec3 vector)
{
    this->transform->Move(vector);
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::Rotate(glm::vec3 vector)
{
    this->transform->Rotate(vector);
    SpotLightComponent::needUpdate = true;
}

glm::vec3 SpotLightComponent::GetPosition()
{
    return this->transform->GetPosition();
}

glm::vec3 SpotLightComponent::GetDirection()
{
    return this->transform->GetRotation();
}

glm::vec3 SpotLightComponent::GetLightColor()
{
    return this->lightColor;
}

glm::vec3 SpotLightComponent::GetSpecularColor()
{
    return this->specularColor;
}

glm::vec3 SpotLightComponent::GetRange()
{
    return this->distance;
}

float SpotLightComponent::GetCutOff()
{
    return this->cutOff;
}

void SpotLightComponent::Use(std::shared_ptr<ShaderComponent> shader)
{
    if(!SpotLightComponent::needUpdate || !this->wasCreated)
    {
        return;
    }

    textID = std::to_string(SpotLightComponent::thisLightID);
    shader->SetInt("spotLightAmount", SpotLightComponent::lightAmount);
    shader->SetVec3("spotLight[" + textID + "].position", this->transform->GetPosition());
    shader->SetVec3("spotLight[" + textID + "].direction", this->transform->GetRotation());
    shader->SetVec3("spotLight[" + textID + "].lightColor", this->lightColor);
    shader->SetVec3("spotLight[" + textID + "].specularColor", this->specularColor);
    shader->SetVec3("spotLight[" + textID + "].distanceVec", this->distance);
    shader->SetFloat("spotLight[" + textID + "].cutOff", this->cutOff);

    if(SpotLightComponent::thisLightID == SpotLightComponent::lightAmount)
    {
        SpotLightComponent::thisLightID = 0;
        SpotLightComponent::needUpdate = false;
    }
    else
    {
        SpotLightComponent::thisLightID++;
    }
}