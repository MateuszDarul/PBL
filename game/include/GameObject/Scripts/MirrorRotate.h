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

private:

    std::shared_ptr<cmp::Transform> transform;

public:
    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>();
    }

    void Update(float dt)
    {
        float modifier = (Input()->Keyboard()->IsPressed(KeyboardKey::LShift)) ? rotationSpeedModifier : 1.0f;
        float rotationAmount = rotationSpeed * modifier * dt;

        if (Input()->Keyboard()->IsPressed(KeyboardKey::W)) currentRotationX -= rotationAmount;
        if (Input()->Keyboard()->IsPressed(KeyboardKey::S)) currentRotationX += rotationAmount;
        if (Input()->Keyboard()->IsPressed(KeyboardKey::D)) currentRotationY -= rotationAmount;
        if (Input()->Keyboard()->IsPressed(KeyboardKey::A)) currentRotationY += rotationAmount;

        currentRotationX = std::clamp(currentRotationX, -maxRotationX, maxRotationX);
        currentRotationY = std::clamp(currentRotationY, -maxRotationY, maxRotationY);

        transform->SetRotation(currentRotationX, currentRotationY, 0.0f);
    }
};