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

    bool disableRotation = false;

private:

    std::shared_ptr<cmp::Transform> transform;

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

        disableRotation = false;
    }

    void Update(float dt)
    {
        glm::vec2 mouseOffset = currentMousePosition - previousMousePosition;
        previousMousePosition = currentMousePosition;
        currentMousePosition = Input()->Mouse()->GetPosition();
        // printf("Mouse offset: %f %f\n", mouseOffset.x, mouseOffset.y);

        if (disableRotation) return;

        float modifier = (Input()->Keyboard()->IsPressed(KeyboardKey::LShift)) ? rotationSpeedModifier : 1.0f;
        currentRotationX -= mouseOffset.y * mouseRotationSpeed * modifier * invertRotationX;
        currentRotationY += mouseOffset.x * mouseRotationSpeed * modifier * invertRotationY;

        currentRotationX = std::clamp(currentRotationX, -maxRotationX+initialRotationOffsetX, maxRotationX+initialRotationOffsetX);
        currentRotationY = std::clamp(currentRotationY, -maxRotationY+initialRotationOffsetY, maxRotationY+initialRotationOffsetY);

        float factor = 0.48f;
        smoothedRotationX = smoothedRotationX + factor * (currentRotationX - smoothedRotationX);
        smoothedRotationY = smoothedRotationY + factor * (currentRotationY - smoothedRotationY);

        transform->SetRotation(smoothedRotationX, smoothedRotationY, 0.0f);


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