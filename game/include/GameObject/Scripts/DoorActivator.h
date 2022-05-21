#pragma once

#include "Components.h"

class DoorActivator : public Script 
{
public:

    //adjust these
    glm::vec3 openedOffset = { 0.0f, 6.0f, 0.0f };
    float doorSpeed = 7.0f;

    //set these in 'inspector'

    std::shared_ptr<cmp::Transform> doorTransform;

private:

    glm::vec3 targetPosition;

    bool isPrimed;
    bool isActivated;

public:

    void Start()
    {
        isPrimed = true;
        isActivated = false;

        if (doorTransform) targetPosition = doorTransform->GetPosition();
    }

    void Update(float dt)
    {
        if (isActivated)    //on being powered
        {
            isActivated = false;

            
        }
        else if (!isPrimed)  //on power off
        {
            isPrimed = true;

            if (doorTransform) targetPosition -= openedOffset;
        }
        else                //on being unpowered
        {
            
        }

        if (doorTransform)
        {
            glm::vec3 move = targetPosition - doorTransform->GetPosition();
            doorTransform->Move(move * doorSpeed * dt);
        }
    }

    void Activate()
    {
        if (isPrimed) //on power on
        {
            isPrimed = false;

            if (doorTransform) targetPosition += openedOffset;
        }

        isActivated = true;
    }
};