#include "ModelInstancesComponent.h"

ModelInstancesComponent::ModelInstancesComponent()
    :Component(4)
{
    
}

ModelInstancesComponent::~ModelInstancesComponent()
{

}

bool ModelInstancesComponent::Create(uint32_t amount, Mesh* mesh, Material* material)
{
    this->amount = amount;
    this->mesh = mesh;
    this->material = material;

    this->transformations = new glm::mat4 [this->amount];

    int rl = (int)sqrt(this->amount);
    int x=0, y=0;
    for(uint32_t i=0; i<this->amount; i++)
    {
        this->transformations[i] = glm::translate(glm::mat4(1.0f), glm::vec3(x*5, 0, y*5));

        x++;
        if(x % rl == 0)
        {
            x=0;
            y++;
        }
    }

    

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER, this->mesh->vertices.size() * sizeof(Mesh::Vertex), &this->mesh->vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);

    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));

    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, texCoords));



    glGenBuffers(1, &this->instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, this->amount * sizeof(glm::mat4), &this->transformations[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

bool ModelInstancesComponent::Draw(std::shared_ptr<ShaderComponent> shader)
{
    if(this->mesh == nullptr || shader == nullptr)
    {
        return false;
    }

    shader->Use();
    shader->SetBool("inst", true);

    if(this->material != nullptr)
    {
        shader->SetInt("diffuseMapData", 0);
        shader->SetInt("specularMapData", 1);
        shader->SetInt("normalMapData", 2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->material->diffuse_texture_id);
        glActiveTexture(GL_TEXTURE1);       
        glBindTexture(GL_TEXTURE_2D, this->material->specular_texture_id);
        glActiveTexture(GL_TEXTURE2);       
        glBindTexture(GL_TEXTURE_2D, this->material->normal_texture_id);
    }

    glBindVertexArray(this->VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, this->mesh->vertices.size(), this->amount);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);

    return true;
}

void ModelInstancesComponent::UpdateTransformations()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
    void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &this->transformations[0], this->amount * sizeof(glm::mat4));
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

uint32_t ModelInstancesComponent::GetAmount()
{
    return this->amount;
}

bool ModelInstancesComponent::SetTransformation(uint32_t instanceID, glm::mat4 newTransformations)
{
    if(instanceID < 0 || instanceID >= this->amount)
    {
        return false;
    }

    this->transformations[instanceID] = newTransformations;

    return true;
}

bool ModelInstancesComponent::SetTransformation(uint32_t instanceID, glm::vec3 position, glm::vec3 rotation, float scale)
{
    if(instanceID < 0 || instanceID >= this->amount)
    {
        return false;
    }

    glm::mat4 transform = glm::mat4(1.f);

    transform = glm::translate(transform, position);
    transform = glm::rotate(transform, (float)rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, (float)rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, (float)rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scale, scale, scale));

    this->transformations[instanceID] = transform;
    
    return true;
}

glm::mat4 ModelInstancesComponent::GetTransformation(uint32_t instanceID)
{
    if(instanceID < 0 || instanceID >= this->amount)
    {
        return glm::mat4(1.f);
    }

    return this->transformations[instanceID];
}