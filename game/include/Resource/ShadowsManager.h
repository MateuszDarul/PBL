#ifndef __SHADOWS_MANAGER_H__
#define __SHADOWS_MANAGER_H__

#include <memory>
#include <vector>

#include "SceneNode.h"
#include "GameObject.h"
#include "ShaderComponent.h"
#include "PointLightComponent.h"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

class Shadow
{
public:
    unsigned int depthMapFBO;
    unsigned int depthCubemap;

public:
    Shadow();
    ~Shadow();
    void Create();
    void Update(std::shared_ptr<ShaderComponent> shader_s, glm::vec3 lightPos, SceneNode* world);
};

///***

class ShadowsManager
{
private: 
    SceneNode* world;
    std::shared_ptr<ShaderComponent> shader_s;
    std::shared_ptr<ShaderComponent> shader_l;

    std::vector<int> CPUID;
    std::vector<GameObject*> lights;
    std::vector<Shadow*> shadows;

    int iterator;

    bool enable[10];
    glm::vec3 pos[10];
    unsigned int depthCubemap[10];

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