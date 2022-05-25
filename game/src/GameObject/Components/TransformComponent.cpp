#include "TransformComponent.h"
TransformComponent::TransformComponent()
    :Component(6)
{
    this->modelMatrix = glm::mat4(1.f);

    this->position = glm::vec3(0,0,0);
    this->rotation = glm::vec3(0,0,0);
    this->scale = glm::vec3(1,1,1);
}

TransformComponent::~TransformComponent()
{

}

const glm::mat4& TransformComponent::GetModelMatrix() const
{
    return this->modelMatrix;
}

void TransformComponent::SetModelMatrix(const glm::mat4& modelMatrix)
{
    this->modelMatrix = modelMatrix;
}

void TransformComponent::CalculateModelMatrix()
{
    this->modelMatrix = 
        glm::translate(glm::mat4(1.f), this->position)*
        glm::rotate(glm::mat4(1.f), glm::radians(this->rotation.z), glm::vec3(0,0,1))*
        glm::rotate(glm::mat4(1.f), glm::radians(this->rotation.y), glm::vec3(0,1,0))*
        glm::rotate(glm::mat4(1.f), glm::radians(this->rotation.x), glm::vec3(1,0,0))*
        glm::scale(glm::mat4(1.f), scale);
}

void TransformComponent::SetScale(const float& scale)
{
    this->scale = glm::vec3(scale,scale,scale);
    this->CalculateModelMatrix();
}

void TransformComponent::SetScale(const glm::vec3 scale)
{
    this->scale = scale;
    this->CalculateModelMatrix();
}

void TransformComponent::Scale(const float& scale)
{
    this->scale *= scale;
    this->CalculateModelMatrix();
}

float TransformComponent::GetScale()
{
    return this->scale.x;
}

glm::vec3 TransformComponent::GetScaleVec()
{
    return this->scale;
}

void TransformComponent::SetPosition(const float& x, const float& y, const float& z)
{
    this->SetPosition(glm::vec3(x, y, z));
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
    this->position = position;
    this->CalculateModelMatrix();
}

void TransformComponent::Move(const float& x, const float& y, const float& z)
{
    this->Move(glm::vec3(x, y, z));
}

void TransformComponent::Move(const glm::vec3& vector)
{
    this->position += vector;
    this->CalculateModelMatrix();
}

glm::vec3 TransformComponent::GetPosition()
{
    return this->position;
}

void TransformComponent::SetRotation(const float& x, const float& y, const float& z)
{
    this->SetRotation(glm::vec3(x, y, z));
}

void TransformComponent::SetRotation(const glm::vec3& degrees)
{
    this->rotation = degrees;
    this->CalculateModelMatrix();
}

void TransformComponent::Rotate(const float& x, const float& y, const float& z)
{
    this->Rotate(glm::vec3(x, y, z));
}

void TransformComponent::Rotate(const glm::vec3& degrees)
{
    this->rotation += degrees;
    this->CalculateModelMatrix();
}

glm::vec3 TransformComponent::GetRotation()
{
    return this->rotation;
}



glm::mat4 TransformComponent::Transform(const float& px, const float& py, const float& pz, const float& rx=0, const float& ry=0, const float& rz=0, const float& scale=1)
{
    return TransformComponent::Transform(glm::vec3(px, py, pz), glm::vec3(rx, ry, rz), scale);
}

glm::mat4 TransformComponent::Transform(const glm::vec3& position, const glm::vec3& rotation, const float& scale)
{
    glm::mat4 result =
        glm::translate(glm::mat4(1.f), position)*
        glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0,0,1))*
        glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0,1,0))*
        glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1,0,0))*
        glm::scale(glm::mat4(1.f), glm::vec3(scale, scale, scale));

    return result;
}