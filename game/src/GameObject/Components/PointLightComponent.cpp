#include "PointLightComponent.h"

PointLightComponent::PointLightComponent(int pointLightID)
    :Component(8), pointLightID(pointLightID)
{
    position = glm::vec3(0,5,0);
    lightColor = glm::vec3(0.5,0.5,0.5);
    specularColor = glm::vec3(1,1,1);
    distance = glm::vec3(1,1,1);
}

PointLightComponent::~PointLightComponent()
{

}
    
void PointLightComponent::Use(std::shared_ptr<ShaderComponent> shader)
{
    shader->SetInt("enablePointLights", 1);
    shader->SetVec3("pointLight.position", position);
    shader->SetVec3("pointLight.lightColor", lightColor);
    shader->SetVec3("pointLight.specularColor", specularColor);
    shader->SetVec3("pointLight.distance", distance);
}