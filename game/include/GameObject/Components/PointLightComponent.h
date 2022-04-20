#ifndef __POINT_LIGHT_COMPONNET_H__
#define __POINT_LIGHT_COMPONNET_H__

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Component.h"
#include "ShaderComponent.h"

class PointLightComponent :public Component
{
public:
    static int lightAmount;
    static int thisLightID;
    static bool needUpdate;

    bool wasCreated;
    std::string textID;

    glm::vec3 position;

    glm::vec3 lightColor;
    glm::vec3 specularColor;

    glm::vec3 distance;
    // distance.x = constant;
    // distance.y = linear;
    // distance.z = quadratic;
    float range;

public:
    PointLightComponent();
    ~PointLightComponent();

    bool Create();
    bool Destroy();

    void Use(std::shared_ptr<ShaderComponent> shader);
};

#endif // __POINT_LIGHT_COMPONNET_H__