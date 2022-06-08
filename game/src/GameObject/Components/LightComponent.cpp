#include "LightComponent.h"

LightComponent::LightComponent(uint32_t component_type_uuid)
    :Component(component_type_uuid)
{

}

LightComponent::~LightComponent()
{

}

bool LightComponent::AddShader(std::shared_ptr<ShaderComponent> shader)
{
    for(uint8_t i=0; i<shaders.size(); i++)
    {
        if(shaders[i] == shader)
        {
            return false;
        }
    }
    
    shaders.push_back(shader);
    return true;
}

void LightComponent::TurnOn()
{
    turnedOn = true;
}

void LightComponent::TurnOff()
{
    turnedOn = false;
}

bool LightComponent::IsTurnedOn()
{
    return turnedOn;
}

std::vector<std::shared_ptr<ShaderComponent>> LightComponent::GetShaders()
{
    return this->shaders;
}