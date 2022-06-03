#pragma once

#include "Components.h"



class MirrorRotate : public Script
{
public:

    float maxRotationX = 30.0f;
    float maxRotationY = 45.0f;

    float currentRotationX = 0.0f;
    float currentRotationY = 0.0f;

    float rotationSpeed = 12.5f;
    float rotationSpeedModifier = 0.088f;

    glm::vec2 currentMousePosition;
    glm::vec2 previousMousePosition;

    float mouseRotationSpeed = 0.069f;

    bool disableRotation = false;

private:

    std::shared_ptr<cmp::Transform> transform;

public:
    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>();
        currentRotationX = transform->GetRotation().x;
        currentRotationY = transform->GetRotation().y;

        currentMousePosition = Input()->Mouse()->GetPosition();
        previousMousePosition = currentMousePosition;

        disableRotation = false;
    }

    void Update(float dt)
    {
        glm::vec2 mouseOffset = currentMousePosition - previousMousePosition;
        previousMousePosition = currentMousePosition;
        currentMousePosition = Input()->Mouse()->GetPosition();
        // printf("Mouse offset: %f %f\n", mouseOffset.x, mouseOffset.y);

        if (disableRotation) return;

        currentRotationX -= mouseOffset.y * mouseRotationSpeed;
        currentRotationY += mouseOffset.x * mouseRotationSpeed;

        currentRotationX = std::clamp(currentRotationX, -maxRotationX, maxRotationX);
        currentRotationY = std::clamp(currentRotationY, -maxRotationY, maxRotationY);

        transform->SetRotation(currentRotationX, currentRotationY, 0.0f);


        // float modifier = (Input()->Keyboard()->IsPressed(KeyboardKey::LShift)) ? rotationSpeedModifier : 1.0f;
        // float rotationAmount = rotationSpeed * modifier * dt;

        // if (Input()->Keyboard()->IsPressed(KeyboardKey::W)) currentRotationX += rotationAmount;
        // if (Input()->Keyboard()->IsPressed(KeyboardKey::S)) currentRotationX -= rotationAmount;
        // if (Input()->Keyboard()->IsPressed(KeyboardKey::D)) currentRotationY -= rotationAmount;
        // if (Input()->Keyboard()->IsPressed(KeyboardKey::A)) currentRotationY += rotationAmount;

        // currentRotationX = std::clamp(currentRotationX, -maxRotationX, maxRotationX);
        // currentRotationY = std::clamp(currentRotationY, -maxRotationY, maxRotationY);

        // transform->SetRotation(currentRotationX, currentRotationY, 0.0f);
    }
};