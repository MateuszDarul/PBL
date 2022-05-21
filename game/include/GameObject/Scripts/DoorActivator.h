#pragma once

#include "Components.h"

class DoorActivator : public Script 
{
public:

    bool isPrimed;

    void Start()
    {
        isPrimed = true;
    }

    void Update(float dt)
    {

    }

    void Activate()
    {
        if (isPrimed)
        {
            isPrimed = false;
            printf("DOOR TRIGGERED\n");
        }
    }
};