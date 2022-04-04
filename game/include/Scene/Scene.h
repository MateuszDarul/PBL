#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include <glm/matrix.hpp>

#include "SceneNode.h"
#include "GameObject.h"
#include "Components.h"

#include "GameApplication.h"
#include "ResourceManager.h"
#include "InputManager.h"

class Scene
{
private:
    SceneNode* scene;
    GameObject goCamera;

    glm::mat4 transform;

public:
    Scene();
    ~Scene();
    void OnUpdate(float dt);
};

#endif // __SCENE_H__