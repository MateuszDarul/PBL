#pragma once

#include "GameApplication.h"
#include "Components.h"

#include "Scripts/StatsScript.h"

#include <iostream>


class TestScript : public Script
{
public:
    StatsScript* stats;

    void Start()
    {
        auto name = gameObject->GetComponent<NameComponent>();
        std::cout << "TestScript of " << (name ? name->Get() : "unnamed") << " started" << std::endl;
    }

    void Update(float dt)
    {
        if (Input()->Keyboard()->OnPressed(P))
        {
            stats->Print();
        }

        if (Input()->Keyboard()->IsPressed(Right))
        {
            stats->changeMe += dt;
        }
        if (Input()->Keyboard()->IsPressed(Left))
        {
            stats->changeMe -= dt;
        }
    }
};