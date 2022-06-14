#include "CameraComponent.h"

#define ENABLE_JUMP_SETTINGS

CameraComponent::CameraComponent()
    :Component(7)
{
    
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::UpdateVectors()
{
    float combinedYaw = yaw + yawOffset;
    float combinedPitch = pitch + pitchOffset;
    if (combinedPitch > 89.0f)
        combinedPitch = 89.0f;
    if (combinedPitch < -89.0f)
        combinedPitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(combinedYaw)) * cos(glm::radians(combinedPitch));
    front.y = sin(glm::radians(combinedPitch));
    front.z = sin(glm::radians(combinedYaw)) * cos(glm::radians(combinedPitch));

    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, glm::vec3(0,1,0)));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
    
bool CameraComponent::Create(const glm::vec3& position)
{
    this->needUpdate = true;
    this->firstMouseMovement = true;
    this->mouseSensitivity = 0.1;
    this->speedPerSec = 12.5f;
    this->jumpHeight = 2.0f;
    this->jumpTimeToPeak = 0.299f;

    this->yaw = -90;
    this->pitch = 0;

    this->yawOffset = 0.0f;
    this->pitchOffset = 0.0f;

    CalculateJumpParams();
    isEnabledMovement = true;
    isEnabledRotation = true;
    
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


//float JUMP_TIMER = 0.0f; //temporary 'is grounded' check

void CameraComponent::CalculateJumpParams()
{
    jumpVelocity = 2 * jumpHeight / jumpTimeToPeak;
    gravity = - 2 * jumpHeight / (jumpTimeToPeak * jumpTimeToPeak);

    printf("Height: %f\tTime: %f\tGravity: %f\tV0: %f\n", jumpHeight, jumpTimeToPeak, gravity, jumpVelocity);
}

void CameraComponent::Update(InputManager* inputManager, const float& deltaTime)
{
#ifdef ENABLE_JUMP_SETTINGS
    if(inputManager->Keyboard()->IsPressed(KeyboardKey::Up))    SetJumpHeight(jumpHeight + deltaTime);
    if(inputManager->Keyboard()->IsPressed(KeyboardKey::Down))  SetJumpHeight(jumpHeight - deltaTime);
    if(inputManager->Keyboard()->IsPressed(KeyboardKey::Right)) SetJumpTimeToPeak(jumpTimeToPeak + deltaTime);
    if(inputManager->Keyboard()->IsPressed(KeyboardKey::Left))  SetJumpTimeToPeak(jumpTimeToPeak - deltaTime);
#endif


    this->ProcessMouseMovement(inputManager->Mouse()->GetPosition());

    if (isEnabledMovement)
    {        
        float velocity = this->speedPerSec * deltaTime;

        if(inputManager->Keyboard()->IsPressed(KeyboardKey::W))
        {
            this->position += glm::normalize(glm::vec3(this->front.x, 0, this->front.z)) * velocity;
            this->needUpdate = true;
        }
        if(inputManager->Keyboard()->IsPressed(KeyboardKey::S))
        {
            this->position -= glm::normalize(glm::vec3(this->front.x, 0, this->front.z)) * velocity;
            this->needUpdate = true;
        }
        if(inputManager->Keyboard()->IsPressed(KeyboardKey::A))
        {
            this->position -= glm::normalize(glm::vec3(this->right.x, 0, this->right.z)) * velocity;
            this->needUpdate = true;
        }
        if(inputManager->Keyboard()->IsPressed(KeyboardKey::D))
        {
            this->position += glm::normalize(glm::vec3(this->right.x, 0, this->right.z)) * velocity;
            this->needUpdate = true;
        }
    }



    if (!isGrounded)
    {
        verticalVelocity += gravity * deltaTime;
        verticalVelocity = std::max(verticalVelocity, gravity);
    }
    else
    {
        verticalVelocity = 0.0f; 
    
        if(inputManager->Keyboard()->IsPressed(KeyboardKey::Space) && isEnabledMovement)
        {
            verticalVelocity = jumpVelocity;
        }
    }

    this->position += glm::vec3(0.0f, verticalVelocity * deltaTime, 0.0f);
    this->needUpdate = true;
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

    if (!isEnabledRotation) return;
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

float CameraComponent::GetPitch() const
{
    return pitch;
}

float CameraComponent::GetYaw() const
{
    return yaw;
}

void CameraComponent::SetRotation(float pitch, float yaw)
{
    this->pitch = pitch;
    this->yaw = yaw;

    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;
    this->UpdateVectors();
    this->needUpdate = true;
}

void CameraComponent::SetRotationOffset(float pitch, float yaw)
{
    this->pitchOffset = pitch;
    this->yawOffset = yaw;

    this->UpdateVectors();
    this->needUpdate = true;
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

float CameraComponent::GetGravity()
{
    return gravity;
}

float CameraComponent::GetJumpVelocity()
{
    return jumpVelocity;
}

float CameraComponent::GetJumpHeight()
{
    return jumpHeight;
}

void  CameraComponent::SetJumpHeight(float newHeight)
{
    jumpHeight = std::max(0.1f, newHeight);
    CalculateJumpParams();
}

float CameraComponent::GetJumpTimeToPeak()
{
    return jumpTimeToPeak;
}

void  CameraComponent::SetJumpTimeToPeak(float newTime)
{
    jumpTimeToPeak = std::max(0.1f, newTime);
    CalculateJumpParams();
}

bool CameraComponent::GetMovementEnabled()
{
    return isEnabledMovement;
}

void CameraComponent::SetMovementEnable(bool enabled)
{
    isEnabledMovement = enabled;
}

bool CameraComponent::GetRotationEnabled()
{
    return isEnabledRotation;
}

void CameraComponent::SetRotationEnable(bool enabled)
{
    isEnabledRotation = enabled;
}

bool CameraComponent::GetIsGrounded()
{
    return this->isGrounded;
}

void CameraComponent::SetIsGrounded(bool grounded)
{
    this->isGrounded = grounded;
}