#ifndef __POINT_LIGHT_COMPONNET_H__
#define __POINT_LIGHT_COMPONNET_H__

#include <memory>

#include <glm/glm.hpp>

#include "Component.h"
#include "ShaderComponent.h"

class PointLightComponent :public Component
{
private:
    std::string var[6];

    int pointLightID;

    glm::vec3 position;

    glm::vec3 lightColor;
    glm::vec3 specularColor;

    glm::vec3 distance;
    // distance.x = constant;
    // distance.y = linear;
    // distance.z = quadratic;

public:
    PointLightComponent(int pointLightID = -1);
    ~PointLightComponent();
    void Use(std::shared_ptr<ShaderComponent> shader);
};

#endif // __POINT_LIGHT_COMPONNET_H__