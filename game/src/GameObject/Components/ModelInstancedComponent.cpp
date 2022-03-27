#include "ModelInstancedComponent.h"

ModelInstancedComponent::ModelInstancedComponent()
    :Component(4)
{

}

ModelInstancedComponent::~ModelInstancedComponent()
{

}

bool ModelInstancedComponent::Create(uint32_t amount, Mesh* mesh, Material* material)
{
    this->amount = amount;
    this->mesh = mesh;
    this->material = material;

    int rl = (int)sqrt(this->amount);
    transformations = new glm::mat4 [this->amount];
    int x=0, y=0;
    for(int i=0; i<this->amount; i++)
    {
        transformations[i] = glm::translate(glm::mat4(1.0f), glm::vec3(x*5, 0, y*5));

        x++;
        if(x % rl == 0)
        {
            x=0;
            y++;
        }
    }

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &transformations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, this->mesh->vertices.size() * sizeof(Mesh::Vertex), &this->mesh->vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);

    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));

    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, texCoords));


    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void ModelInstancedComponent::Draw(Shader* shader)
{
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, this->amount);
}