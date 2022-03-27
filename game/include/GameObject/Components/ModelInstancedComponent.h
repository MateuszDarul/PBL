#ifndef __MODEL_INSTANCED_COMPONENT_H__
#define __MODEL_INSTANCED_COMPONENT_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"

class ModelInstancedComponent :public Component
{
private:
    uint32_t amount;
    uint32_t VAO;
    uint32_t VBO;
    uint32_t instanceVBO;

    Mesh* mesh;
    Material* material;
    glm::mat4 *transformations;

public:
    ModelInstancedComponent();
    ~ModelInstancedComponent();

    bool Create(uint32_t amount, Mesh* mesh, Material* material);
    void Draw(Shader* shader);
};

#endif // __MODEL_INSTANCED_COMPONENT_H__