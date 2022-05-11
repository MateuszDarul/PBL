#ifndef __SPOT_LIGHT_COMPONENT_H__
#define __SPOT_LIGHT_COMPONENT_H__

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Component.h"
#include "TransformComponent.h"
#include "ShaderComponent.h"

class SpotLightComponent :public Component
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
    glm::vec3 distance;
    float cutOff;

public:
    SpotLightComponent();
    ~SpotLightComponent();

    bool Create();
    bool Destroy();

    void SetPosition(glm::vec3 position);
    void SetDirection(glm::vec3 direction);
    void SetLightColor(glm::vec3 lightColor);
    void SetSpecularColor(glm::vec3 specularColor);
    void SetRange(float constant, float linear, float quadratic);
    void SetCutOff(float cutOff);

    void Move(glm::vec3 vector);
    void Rotate(glm::vec3 vector);

    glm::vec3 GetPosition();
    glm::vec3 GetDirection();
    glm::vec3 GetLightColor();
    glm::vec3 GetSpecularColor();
    glm::vec3 GetRange();
    float GetCutOff();

    void Use(std::shared_ptr<ShaderComponent> shader);
};

#endif // __SPOT_LIGHT_COMPONENT_H__