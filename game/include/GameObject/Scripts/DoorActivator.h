#pragma once

#include "Components.h"

class DoorActivator : public Script 
{
public:

    //adjust these
    glm::vec3 openedOffset = { 0.0f, 6.0f, 0.0f };

    //set these in 'inspector'

    std::shared_ptr<cmp::Transform> doorTransform;

private:

    bool isPrimed;
    bool isActivated;

public:

    void Start()
    {
        isPrimed = true;
        isActivated = false;
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

            if (doorTransform) doorTransform->Move(-openedOffset);
        }
        else                //on being unpowered
        {
            
        }
    }

    void Activate()
    {
        if (isPrimed) //on power on
        {
            isPrimed = false;

            if (doorTransform) doorTransform->Move(openedOffset);
        }

        isActivated = true;
    }
};