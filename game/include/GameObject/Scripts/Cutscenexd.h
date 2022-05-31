#pragma once

#include "Components.h"
#include "DoorActivator.h"


class Cutscenexd : public Script
{
public:

    std::shared_ptr<cmp::PointLight> light;

    DoorActivator* activator;

    void Start()
    {
        
    }

    void Update(float dt)
    {
        
    }

    void TriggerEnter(std::shared_ptr<ColliderComponent> other) override
    {
        if (other->layer & CollisionLayer::Player)
        {
            // Player entered trigger
            
            activator->targetPosition.y -= 10.0f;
        }
    }
};