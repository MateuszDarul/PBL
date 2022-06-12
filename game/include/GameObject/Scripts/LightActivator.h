#pragma once

#include "Components.h"
#include "GameObject.h"

#include "LanternRange.h"

class LightActivator : public Script
{
public:

    cmp::Model* bulbModel;
    cmp::PointLight* lightComponent;
    LanternRange* range;

    bool isAlwaysLit = false;

private:
    std::shared_ptr<cmp::Transform> transform;

    bool isPrimed;
    bool isActivated;
    bool isShutdown;

public:

    void Start()
    {
        isPrimed = true;
        isActivated = false;
        isShutdown = false;

        transform = gameObject->GetComponent<cmp::Transform>();
    }

    void Update(float dt)
    {
        if (isAlwaysLit) return;

        if (isShutdown)
        {
            isActivated = false;
            isPrimed = true;
        }
        else if (isActivated)    //on being powered
        {
            isActivated = false;


        }
        else if (!isPrimed)  //on power off
        {
            isPrimed = true;

            if (bulbModel) bulbModel->SetTintColor(0.4f, 0.4f, 0.8f);
            if (lightComponent) lightComponent->SetPosition({ 999.9f, 999.9f, 999.9f });
            if (range) range->ChangeLightPower(false);
        }
        else                //on being unpowered
        {

        }

        
    }

    void Activate()
    {
        if (!isShutdown)
        {
            if (isPrimed) //on power on
            {
                isPrimed = false;

                if (bulbModel) bulbModel->SetTintColor(1.0f, 1.0f, 1.0f);
                if (lightComponent) lightComponent->SetPosition(transform->GetPosition());
                if (range) range->ChangeLightPower(true);
            }

            isActivated = true;
        }
    }

    void ForceShutdown(bool shutdown = true)
    {
        isShutdown = shutdown;
        if (isShutdown)
        {
            
        }
    }
};