#include "ShadowsManager.h"

Shadow::Shadow()
{

}

Shadow::~Shadow()
{

}

void Shadow::Create()
{
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i=0; i<6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::Update(std::shared_ptr<ShaderComponent> shader_s, glm::vec3 lightPos, SceneNode* world)
{
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, 1.f, 100.f);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    shader_s->Use();
    for (unsigned int i=0; i<6; i++)
        shader_s->SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    shader_s->SetVec3("lightPos", lightPos);
    world->RenderDepthMap(shader_s);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


///*** --------------------------------------------------------------------------------------------


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
    for(int i=0; i<10; i++)
        shader_l->SetInt("depthMap["+std::to_string(i)+"]", 10+i);
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
    for(int i=0; i<10; i++)
    {
        this->Set(i, false, glm::vec3(0,0,0), 0);
    }
}

void ShadowsManager::Set(int id, bool enable, glm::vec3 pos, unsigned int depthCubemap)
{
    this->enable[id] = enable;
    this->pos[id] = pos;
    this->depthCubemap[id] = depthCubemap;

    shader_l->Use();
    shader_l->SetBool("depthMapStatus["+std::to_string(id)+"]", enable);
    shader_l->SetVec3("depthMapPosition["+std::to_string(id)+"]", pos);
    glActiveTexture(GL_TEXTURE0+10+id);
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
        for(int i=0; i<10; i++)
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