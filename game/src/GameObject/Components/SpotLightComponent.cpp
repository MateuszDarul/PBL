#include "SpotLightComponent.h"

#include "GameObject.h"

int SpotLightComponent::lightAmount = 0;
int SpotLightComponent::thisLightID = 0;
bool SpotLightComponent::needUpdate = false;

SpotLightComponent::SpotLightComponent()
    :LightComponent(9)
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
    this->transform->SetPosition(glm::vec3(0,5,0));
    this->transform->SetRotation(glm::vec3(0,-1,0));
    this->lightColor = glm::vec3(1,1,1);
    this->specularColor = glm::vec3(1,1,1);
    this->damping = 10;
    this->cutOff = glm::vec2(12.5f, 45.f);

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
    this->transform->SetRotation(glm::normalize(direction));
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

void SpotLightComponent::SetDamping(float damping)
{
    this->damping = damping;
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::SetCutOff(glm::vec2 cutOff)
{
    this->cutOff = cutOff;
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::Move(glm::vec3 vector)
{
    this->transform->Move(vector);
    SpotLightComponent::needUpdate = true;
}

void SpotLightComponent::Rotate(glm::vec3 degrees)
{
    glm::vec4 target(1,1,1,1);
    target = glm::vec4(this->transform->GetRotation(), 1);
    target = TransformComponent::Transform(glm::vec3(0,0,0), degrees, 1) * target;
    
    this->transform->SetRotation(glm::vec3(target.x, target.y, target.z));
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

float SpotLightComponent::GetDamping()
{
    return this->damping;
}

glm::vec2 SpotLightComponent::GetCutOff()
{
    return this->cutOff;
}

void SpotLightComponent::Use()
{
    if(!SpotLightComponent::needUpdate || !this->wasCreated)
    {
        return;
    }

    textID = std::to_string(SpotLightComponent::thisLightID);
    for(uint16_t i=0; i<shaders.size(); i++)
    {
        shaders[i]->Use();
        shaders[i]->SetInt("spotLightAmount", SpotLightComponent::lightAmount);
        shaders[i]->SetVec3("spotLight[" + textID + "].position", this->transform->GetPosition());
        shaders[i]->SetVec3("spotLight[" + textID + "].direction", this->transform->GetRotation());
        shaders[i]->SetVec3("spotLight[" + textID + "].lightColor", this->lightColor);
        shaders[i]->SetVec3("spotLight[" + textID + "].specularColor", this->specularColor);
        shaders[i]->SetFloat("spotLight[" + textID + "].distance", this->damping * DAMPING_MUL);
        shaders[i]->SetFloat("spotLight[" + textID + "].cutOff", glm::cos(glm::radians(this->cutOff.x)));
        shaders[i]->SetFloat("spotLight[" + textID + "].outerCutOff", glm::cos(glm::radians(this->cutOff.y)));
    }

    if(SpotLightComponent::thisLightID == SpotLightComponent::lightAmount - 1)
    {
        SpotLightComponent::thisLightID = 0;
        SpotLightComponent::needUpdate = false;
    }
    else
    {
        SpotLightComponent::thisLightID++;
    }
}