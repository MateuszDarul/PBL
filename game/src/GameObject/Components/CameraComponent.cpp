#include "CameraComponent.h"

CameraComponent::CameraComponent()
    :Component(7)
{
    this->needUpdate = true;
    this->firstMouseMovement = true;
    this->mouseSensitivity = 0.1;
    this->speedPerSec = 1;

    this->yaw = -90;
    this->pitch = 0;
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::UpdateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, glm::vec3(0,1,0)));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
    
bool CameraComponent::Create(const glm::vec3& position)
{
    this->position = position;

    UpdateVectors();

    return true;
}

float CameraComponent::GetMouseSensitivity()
{
    return this->mouseSensitivity;
}

void CameraComponent::SetMouseSensitivity(const float& mouseSensitivity)
{
    this->mouseSensitivity = mouseSensitivity;
}

float CameraComponent::GetSpeed()
{
    return this->speedPerSec;
}

void CameraComponent::SetSpeed(const float& speedPerSec)
{
    this->speedPerSec = speedPerSec;
}

const glm::mat4& CameraComponent::GetView()
{
    if(this->needUpdate)
    {
        this->view = glm::lookAt(this->position, this->position + this->front, this->up);
    }
    return this->view;
}

void CameraComponent::Update(InputManager* inputManager, const float& deltaTime)
{
    this->ProcessMouseMovement(inputManager->Mouse()->GetPosition());

    if(inputManager->Keyboard()->IsPressed(KeyboardKey::W))
    {
        this->Move(Movement::FORWARD, this->speedPerSec, deltaTime);
    }
    if(inputManager->Keyboard()->IsPressed(KeyboardKey::S))
    {
        this->Move(Movement::BACKWARD, this->speedPerSec, deltaTime);
    }
    if(inputManager->Keyboard()->IsPressed(KeyboardKey::A))
    {
        this->Move(Movement::LEFT, this->speedPerSec, deltaTime);
    }
    if(inputManager->Keyboard()->IsPressed(KeyboardKey::D))
    {
        this->Move(Movement::RIGHT, this->speedPerSec, deltaTime);
    }
    if(inputManager->Keyboard()->IsPressed(KeyboardKey::Q))
    {
        this->Move(Movement::UP, this->speedPerSec, deltaTime);
    }
    if(inputManager->Keyboard()->IsPressed(KeyboardKey::E))
    {
        this->Move(Movement::DOWN, this->speedPerSec, deltaTime);
    }
}

void CameraComponent::Move(Movement direction, const float& offset, const float& deltaTime)
{
    float velocity = offset * deltaTime;

    if(direction == Movement::UP)
        this->position += this->up * velocity;
    if(direction == Movement::DOWN)
        this->position -= this->up * velocity;
    if(direction == Movement::LEFT)
        this->position -= this->right * velocity;
    if(direction == Movement::RIGHT)
        this->position += this->right * velocity;
    if(direction == Movement::FORWARD)
        this->position += this->front * velocity;
    if(direction == Movement::BACKWARD)
        this->position -= this->front * velocity;

    this->needUpdate = true;
}

void CameraComponent::ProcessMouseMovement(const glm::vec2& offset)
{
    ProcessMouseMovement(offset.x, offset.y);
}

void CameraComponent::ProcessMouseMovement(const float& offsetX, const float& offsetY)
{
    if(this->firstMouseMovement)
    {
        this->lastCursorPosition.x = offsetX;
        this->lastCursorPosition.y = offsetY;
        this->firstMouseMovement = false;
    }

    float xoffset = offsetX - this->lastCursorPosition.x;
    float yoffset = this->lastCursorPosition.y - offsetY;

    this->lastCursorPosition.x = offsetX;
    this->lastCursorPosition.y = offsetY;

    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    if(this->pitch > 89.0f)
        this->pitch = 89.0f;
    if(this->pitch < -89.0f)
        this->pitch = -89.0f;

    this->UpdateVectors();
    this->needUpdate = true;
}

glm::vec3 CameraComponent::GetPosition()
{
    return position;
}

void CameraComponent::SetPosition(const glm::vec3& position)
{
    this->position = position;
}


Frustum CameraComponent::GetFrustum(float aspect, float fov, float nearPlane, float farPlane)
{
    Frustum frustum;
    float halfVSide = farPlane * tanf(fov * 0.5f);
    float halfHSide = halfVSide * aspect;
    glm::vec3 frontMultFar = farPlane * this->front;

    frustum.Create(
        Plane(this->position, glm::cross(this->right, frontMultFar - this->up * halfVSide)),
        Plane(this->position, glm::cross(frontMultFar + this->up * halfVSide, this->right)),
        Plane(this->position, glm::cross(frontMultFar - this->right * halfHSide, this->up)),
        Plane(this->position, glm::cross(this->up,frontMultFar + this->right * halfHSide)),
        Plane(this->position + frontMultFar, -this->front),
        Plane(this->position + nearPlane * this->front, this->front)
    );

    return frustum;
}