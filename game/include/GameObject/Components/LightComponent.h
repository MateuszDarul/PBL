#ifndef __LIGHT_COMPONENT_H__
#define __LIGHT_COMPONENT_H__

#include <memory>
#include <string>

#include "Component.h"
#include "TransformComponent.h"
#include "ShaderComponent.h"

class LightComponent :public Component
{
protected:
    std::string textID;
    bool wasCreated;

    std::shared_ptr<TransformComponent> transform;
    glm::vec3 lightColor;
    glm::vec3 specularColor;
    float distance;

public:
    LightComponent(uint32_t component_type_uuid);
    virtual ~LightComponent();

    virtual bool Create() = 0;
    virtual bool Destroy() = 0;
    virtual void Use(std::shared_ptr<ShaderComponent> shader) = 0;
};

#endif // __LIGHT_COMPONENT_H__