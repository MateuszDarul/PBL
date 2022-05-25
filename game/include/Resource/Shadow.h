#ifndef __SHADOW_H__
#define __SHADOW_H__

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneNode.h"
#include "ShaderComponent.h"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

class Shadow
{
public:
    bool wasCreated;
    unsigned int depthMapFBO;
    unsigned int depthCubemap;

public:
    Shadow();
    ~Shadow();
    void Create();
    void Delete();
    void Update(std::shared_ptr<ShaderComponent> shader_s, glm::vec3 lightPos, SceneNode* world);
};

#endif // __SHADOW_H__