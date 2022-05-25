#include "RefractiveGlassComponent.h"

RefractiveGlassComponent::RefractiveGlassComponent()
    :Component(16)
{

}

RefractiveGlassComponent::~RefractiveGlassComponent()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

bool RefractiveGlassComponent::Create(Mesh* mesh, float ratio, unsigned int skyboxTexture)
{
    this->mesh = mesh;
    this->ratio = ratio;
    this->skyboxTexture = skyboxTexture;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER, this->mesh->vertices.size() * sizeof(Mesh::Vertex), &this->mesh->vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));

    glBindVertexArray(0);

    return true;
}

bool RefractiveGlassComponent::Draw(std::shared_ptr<ShaderComponent> shader)
{
    if (this->mesh == nullptr || shader == nullptr)
    {
        return false;
    }

    shader->Use();
    shader->SetFloat("ratio", 1.00f / this->ratio);
    shader->SetInt("skybox", 0);

    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size());

    glBindVertexArray(0);

    return true;
}