#ifndef __POINT_LIGHT_COMPONNET_H__
#define __POINT_LIGHT_COMPONNET_H__

#include <glm/glm.hpp>

#include "LightComponent.h"

class PointLightComponent :public LightComponent
{
private:
    static int lightAmount;
    static int thisLightID;
    static bool needUpdate;

public:
    PointLightComponent();
    ~PointLightComponent();

    bool Create();
    bool Destroy();

    void SetPosition(glm::vec3 position);
    void SetLightColor(glm::vec3 lightColor);
    void SetSpecularColor(glm::vec3 specularColor);
    void SetDamping(float damping);
    
    void Move(glm::vec3 vector);

    glm::vec3 GetPosition();
    glm::vec3 GetLightColor();
    glm::vec3 GetSpecularColor();
    float GetDamping();

    void Use();
};

#endif // __POINT_LIGHT_COMPONNET_H__