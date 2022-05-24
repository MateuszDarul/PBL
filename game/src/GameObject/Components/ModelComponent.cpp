#include "ModelComponent.h"

ModelComponent::ModelComponent()
    :Component(3)
{
    ;
}

ModelComponent::~ModelComponent()
{
    ;
}

bool ModelComponent::Create(Mesh* mesh, Material* material)
{
    if(mesh == nullptr)
    {
        return false;
    }

    this->mesh = mesh;
    this->material = material;

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

    glBindVertexArray(0);

    return true;
}

void ModelComponent::Clear()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);

    this->mesh = nullptr;
    this->material = nullptr;
}

bool ModelComponent::Draw(std::shared_ptr<ShaderComponent> shader) 
{
    if(this->mesh == nullptr || shader == nullptr)
    {
        return false;
    }

    shader->Use();

    if(this->material != nullptr)
    {
        shader->SetInt("diffuseMapData", 0);
        shader->SetInt("specularMapData", 1);
        shader->SetInt("normalMapData", 2);
        shader->SetVec4("u_TintColor", this->tintColor);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->material->diffuse_texture_id);
        glActiveTexture(GL_TEXTURE1);       
        glBindTexture(GL_TEXTURE_2D, this->material->specular_texture_id);
        glActiveTexture(GL_TEXTURE2);       
        glBindTexture(GL_TEXTURE_2D, this->material->normal_texture_id);
    }

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size());

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);

    return true;
}

void ModelComponent::SetTintColor(const glm::vec4& color)
{
    this->tintColor = color;
}

void ModelComponent::SetTintColor(float r, float g, float b, float a)
{
    this->tintColor = {r, g, b, a};
}

const glm::vec4& ModelComponent::GetTintColor()
{
    return this->tintColor;
}