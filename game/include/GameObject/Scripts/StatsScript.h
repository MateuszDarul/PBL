#pragma once

#include "GameApplication.h"
#include "Components.h"

#include <iostream>


class StatsScript : public Script
{
public:
    const float stat1 = 10.0f;
    float stat2 = 14.0f;
    float changeMe = 0.0f;

    void Start()
    {
        
    }

    void Update(float dt)
    {
        
    }

    void Print()
    {
        std::cout << "Stats: "
         << stat1     << ", "
         << stat2     << ", "
         << changeMe  << ", "
        << std::endl;
    }
};