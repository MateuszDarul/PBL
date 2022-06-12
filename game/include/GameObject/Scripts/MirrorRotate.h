#pragma once

#include "Components.h"



class MirrorRotate : public Script
{
public:

    float initialRotationOffsetX = 0.0f;
    float initialRotationOffsetY = 0.0f;

    float maxRotationX = 30.0f;
    float maxRotationY = 45.0f;

    float currentRotationX = 0.0f;
    float currentRotationY = 0.0f;

    float smoothedRotationX;
    float smoothedRotationY;

    float rotationSpeed = 12.5f;
    float rotationSpeedModifier = 0.088f;

    glm::vec2 currentMousePosition;
    glm::vec2 previousMousePosition;

    float mouseRotationSpeed = 0.069f;

    float invertRotationX = 1.0f;
    float invertRotationY = 1.0f;

    bool disableMouseRotation = false;
    bool isPlayerInside = false;

    std::shared_ptr<GameObject> cameraHandle;

private:

    std::shared_ptr<cmp::Transform> transform;
    std::shared_ptr<cmp::Camera> camera;

public:
    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>();
        currentRotationX = transform->GetRotation().x;
        currentRotationY = transform->GetRotation().y;
        smoothedRotationX = currentRotationX;
        smoothedRotationY = currentRotationY;

        currentMousePosition = Input()->Mouse()->GetPosition();
        previousMousePosition = currentMousePosition;

        disableMouseRotation = false;

        camera = gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>();

        if (cameraHandle == nullptr)
        {
            cameraHandle = std::make_shared<GameObject>();
            cameraHandle->AddComponent(std::make_shared<cmp::Transform>());
            cameraHandle->GetComponent<cmp::Transform>()->SetPosition(0.0f, 0.0f, -2.0f);
            gameObject->GetNode()->AddChild(cameraHandle);
        }
    }

    void Update(float dt)
    {
        glm::vec2 mouseOffset = currentMousePosition - previousMousePosition;
        previousMousePosition = currentMousePosition;
        currentMousePosition = Input()->Mouse()->GetPosition();
        // printf("Mouse offset: %f %f\n", mouseOffset.x, mouseOffset.y);
        
        
        float modifier = (Input()->Keyboard()->IsPressed(KeyboardKey::LShift)) ? rotationSpeedModifier : 1.0f;

        if (!disableMouseRotation)
        {
            currentRotationX -= mouseOffset.y * mouseRotationSpeed * modifier * invertRotationX;
            currentRotationY += mouseOffset.x * mouseRotationSpeed * modifier * invertRotationY;

            currentRotationX = std::clamp(currentRotationX, -maxRotationX + initialRotationOffsetX, maxRotationX + initialRotationOffsetX);
            currentRotationY = std::clamp(currentRotationY, -maxRotationY + initialRotationOffsetY, maxRotationY + initialRotationOffsetY);

            float factor = 0.48f;
            smoothedRotationX = smoothedRotationX + factor * (currentRotationX - smoothedRotationX);
            smoothedRotationY = smoothedRotationY + factor * (currentRotationY - smoothedRotationY);

            transform->SetRotation(smoothedRotationX, smoothedRotationY, 0.0f);
        }
        else
        {
            float rotationAmount = rotationSpeed * modifier * dt;

            if (Input()->Keyboard()->IsPressed(KeyboardKey::W)) currentRotationX += rotationAmount * invertRotationX;
            if (Input()->Keyboard()->IsPressed(KeyboardKey::S)) currentRotationX -= rotationAmount * invertRotationX;
            if (Input()->Keyboard()->IsPressed(KeyboardKey::D)) currentRotationY -= rotationAmount * invertRotationY;
            if (Input()->Keyboard()->IsPressed(KeyboardKey::A)) currentRotationY += rotationAmount * invertRotationY;

            currentRotationX = std::clamp(currentRotationX, -maxRotationX + initialRotationOffsetX, maxRotationX + initialRotationOffsetX);
            currentRotationY = std::clamp(currentRotationY, -maxRotationY + initialRotationOffsetY, maxRotationY + initialRotationOffsetY);

            transform->SetRotation(currentRotationX, currentRotationY, 0.0f);

            if (isPlayerInside)
            {
                const auto& m = cameraHandle->GetNode()->GetGlobalTransformations();
                auto newPos = glm::vec3(m[3][0], m[3][1], m[3][2]);
                camera->SetPosition(newPos);
                camera->SetRotationOffset(currentRotationX, -currentRotationY);
                camera->SetIsGrounded(true);
            }
        }
    }
};