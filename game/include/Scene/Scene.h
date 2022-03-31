#pragma once

#include "GameObject.h"
#include "Components.h"

#include <glm/matrix.hpp>

/*
brak poprawnej implementacji,
stworzone tylko po to by GameApplication mialo sie do czego odniesc
*/

class Scene
{
public:
    Scene();
    ~Scene();
    void OnUpdate(float dt);

private:
    ShaderComponent* shader;
    ModelInstancesComponent* mic;
    GameObject go;

    float radius = 10.0f;
    float camX = 0, camZ = 0;
    glm::mat4 projection, view;
    glm::mat4 transform;
};