#ifndef __POINT_LIGHT_COMPONNET_H__
#define __POINT_LIGHT_COMPONNET_H__

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Component.h"
#include "TransformComponent.h"
#include "ShaderComponent.h"

class PointLightComponent :public Component
{
private:
    static int lightAmount;
    static int thisLightID;
    static bool needUpdate;

    std::string textID;
    bool wasCreated;

    std::shared_ptr<TransformComponent> transform;
    glm::vec3 lightColor;
    glm::vec3 specularColor;
    float distance;

public:
    PointLightComponent();
    ~PointLightComponent();

    bool Create();
    bool Destroy();

    void SetPosition(glm::vec3 position);
    void SetLightColor(glm::vec3 lightColor);
    void SetSpecularColor(glm::vec3 specularColor);
    void SetRange(float distance);

    void Move(glm::vec3 vector);

    glm::vec3 GetPosition();
    glm::vec3 GetLightColor();
    glm::vec3 GetSpecularColor();
    float GetRange();

    void Use(std::shared_ptr<ShaderComponent> shader);
};

#endif // __POINT_LIGHT_COMPONNET_H__