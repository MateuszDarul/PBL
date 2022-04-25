#pragma once

#include "GameApplication.h"
#include "Components.h"
#include "GameObject.h"

#include <iostream>


class RotatorScript : public Script
{
public:
    float speed = 10.0f;

    void Start()
    {
        
    }

    void Update(float dt)
    {
        gameObject->GetComponent<TransformComponent>()->Rotate(speed*dt, 0, 0);
    }
};