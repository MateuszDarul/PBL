#ifndef __SPOT_LIGHT_COMPONENT_H__
#define __SPOT_LIGHT_COMPONENT_H__

#include <glm/glm.hpp>

#include "LightComponent.h"

#define DAMPING_MUL 0.004f

class SpotLightComponent :public LightComponent
{
private:
    static int lightAmount;
    static int thisLightID;
    static bool needUpdate;

    glm::vec2 cutOff;

public:
    SpotLightComponent();
    ~SpotLightComponent();

    bool Create();
    bool Destroy();

    void SetPosition(glm::vec3 position);
    void SetDirection(glm::vec3 direction);
    void SetLightColor(glm::vec3 lightColor);
    void SetSpecularColor(glm::vec3 specularColor);
    void SetDamping(float damping);
    void SetCutOff(glm::vec2 cutOff);

    void Move(glm::vec3 vector);
    void Rotate(glm::vec3 degrees);

    glm::vec3 GetPosition();
    glm::vec3 GetDirection();
    glm::vec3 GetLightColor();
    glm::vec3 GetSpecularColor();
    float GetDamping();
    glm::vec2 GetCutOff();

    void Use();
};

#endif // __SPOT_LIGHT_COMPONENT_H__