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
    std::vector<std::shared_ptr<ShaderComponent>> shaders;

    std::string textID;
    bool wasCreated;

    std::shared_ptr<TransformComponent> transform;
    glm::vec3 lightColor;
    glm::vec3 specularColor;
    float damping;

public:
    LightComponent(uint32_t component_type_uuid);
    virtual ~LightComponent();

    virtual bool Create() = 0;
    virtual bool Destroy() = 0;
    virtual void Use() = 0;

    bool AddShader(std::shared_ptr<ShaderComponent> shader);
};

#endif // __LIGHT_COMPONENT_H__