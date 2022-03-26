#ifndef __MODEL_COMPONENT_H__
#define __MODEL_COMPONENT_H__

#include <glad/glad.h>

#include "Component.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

class ModelComponent :public Component
{
private:
    unsigned int VAO;
    unsigned int VBO;

    Mesh* mesh;
    Material* material;

public:
    ModelComponent();
    ~ModelComponent();
    bool Create(Mesh* mesh, Material* material);
    void Clear();
    bool Draw(Shader *shader);
};

#endif // __MODEL_COMPONENT_H__