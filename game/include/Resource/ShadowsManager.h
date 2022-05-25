#ifndef __SHADOWS_MANAGER_H__
#define __SHADOWS_MANAGER_H__

#include <memory>
#include <vector>

#include "SceneNode.h"
#include "GameObject.h"
#include "ShaderComponent.h"
#include "PointLightComponent.h"
#include "Shadow.h"

const unsigned int MAX_SHADOW_MAPS = 10;

class ShadowsManager
{
private: 
    SceneNode* world;
    std::shared_ptr<ShaderComponent> shader_s;
    std::shared_ptr<ShaderComponent> shader_l;

    std::vector<int> CPUID;
    std::vector<GameObject*> lights;
    std::vector<Shadow*> shadows;

    uint8_t iterator;

    bool enable[MAX_SHADOW_MAPS];
    glm::vec3 pos[MAX_SHADOW_MAPS];
    unsigned int depthCubemap[MAX_SHADOW_MAPS];

    void DisableAll();
    void Set(int id, bool enable, glm::vec3 pos, unsigned int depthCubemap);
public:
    ShadowsManager(SceneNode* world, std::shared_ptr<ShaderComponent> shader_l);
    ~ShadowsManager();
    bool AddLight(GameObject* light);
    bool RemoveLight(GameObject* light);
    void Update();
};


#endif // __SHADOWS_MANAGER_H__