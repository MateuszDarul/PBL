#include "ShadowsManager.h"

ShadowsManager::ShadowsManager(SceneNode* world, std::shared_ptr<ShaderComponent> shader_l)
    :world(world), shader_l(shader_l)
{
    iterator = 0;

    shader_s = std::make_shared<ShaderComponent>();
    shader_s->Create(
        "Resources/shaders/shadow.vert", 
        "Resources/shaders/shadow.frag", 
        "Resources/shaders/shadow.geom"
        );

    this->DisableAll();

    shader_l->Use();
    for(int i=0; i<MAX_SHADOW_MAPS; i++)
        shader_l->SetInt("depthMap[" + std::to_string(i) + "]", 10 + i);
}

ShadowsManager::~ShadowsManager()
{
    for(uint32_t i=0; i<shadows.size(); i++)
    {
        delete shadows[i];
    }
    shadows.clear();
}

void ShadowsManager::DisableAll()
{
    for(int i=0; i<MAX_SHADOW_MAPS; i++)
    {
        this->Set(i, false, glm::vec3(0,0,0), 0);
    }

    for(int i=0; i<CPUID.size(); i++)
    {
        CPUID[i] = -1;
    }
}

void ShadowsManager::Set(int id, bool enable, glm::vec3 pos, unsigned int depthCubemap)
{
    this->enable[id] = enable;
    this->pos[id] = pos;
    this->depthCubemap[id] = depthCubemap;

    shader_l->Use();
    shader_l->SetBool("depthMapStatus[" + std::to_string(id) + "]", enable);
    shader_l->SetVec3("depthMapPosition[" + std::to_string(id) + "]", pos);
    glActiveTexture(GL_TEXTURE0 + 10 + id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
}

bool ShadowsManager::AddLight(GameObject* light)
{
    if(light->GetComponent<PointLightComponent>() == nullptr)
    {
        return false;
    }
        
    for(uint32_t i=0; i<lights.size(); i++)
    {
        if(light == lights[i])
        {
            return false;
        }
    }

    CPUID.push_back(-1);
    lights.push_back(light);
    shadows.push_back(new Shadow());
    shadows[shadows.size()-1]->Create();
    shadows[shadows.size()-1]->Update(
        shader_s, 
        light->GetComponent<PointLightComponent>()->GetPosition(),
        world
        );

    return true;
}

bool ShadowsManager::RemoveLight(GameObject* light)
{
    if(light->GetComponent<PointLightComponent>() == nullptr)
    {
        return false;
    }
        
    for(uint32_t i=0; i<lights.size(); i++)
    {
        if(light == lights[i])
        {
            CPUID.erase(CPUID.begin() + i);
            lights.erase(lights.begin() + i);
            delete shadows[i];
            shadows.erase(shadows.begin() + i);

            DisableAll();

            return true;
        }
    }

    return false;
}

void ShadowsManager::Update()
{
    if(shadows.size() == 0)
    {
        return;
    }

    if(iterator >= shadows.size())
    {
        iterator = 0;
    }

    glm::vec3 pos = lights[iterator]->GetComponent<PointLightComponent>()->GetPosition();
    shadows[iterator]->Update(shader_s, pos, world);

    if(CPUID[iterator] == -1)
    {
        for(int i=0; i<MAX_SHADOW_MAPS; i++)
        {
            if(enable[i] == false)
            {
                CPUID[iterator] = i;
                Set(i, true, pos, shadows[iterator]->depthCubemap);
                break;
            }
        }
    }
    else
    {
        Set(CPUID[iterator], true, pos, shadows[iterator]->depthCubemap);
    }

    iterator++;
}