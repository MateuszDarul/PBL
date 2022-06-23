#pragma once

#include "Components.h"


class SwitchLevelTrigger : public Script
{
public:

    int newLevelIndex = 0;

public:

    void Start()
    {

    }

    void Update(float dt)
    {

    }

    void TriggerEnter(std::shared_ptr<ColliderComponent> other) override;
};