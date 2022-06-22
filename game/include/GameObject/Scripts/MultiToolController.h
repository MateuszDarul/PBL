#pragma once

#include "Components.h"



class MultiToolController : public Script
{
public:

    //adjust these
    glm::vec4 activeColor = { 0.78f, 0.78f, 0.78f,  1.0f };
    glm::vec4 inactiveColor = { 0.17f, 0.17f, 0.17f,  1.0f };


    //set in 'inspector'

    std::shared_ptr<GameObject> iconsGO[3];
    std::shared_ptr<GameObject> progressBar;

    std::shared_ptr<cmp::SpotLight> flashlight;


    //for public read
    bool isFlashlightOn = false;
    float maxFlashlightCharge = 10.0f; //10 seconds
    float currentFlashlightCharge;

    //
    int lightSourcesInRange = 0;

private:

    float disabledIconsOffset = 0.1f;

public:

    void Start()
    {
        currentFlashlightCharge = maxFlashlightCharge;

        for (int i = 0; i < 3; i++)
        {
            // 'disabling' icons
            iconsGO[i]->GetComponent<cmp::Transform>()->Move(0.0f, 0.0f, -disabledIconsOffset);
        }
    }
bool manuallyTurnedOn = false;
    void Update(float dt)
    {
        if (lightSourcesInRange > 0) 
        {
            currentFlashlightCharge += 1.618f * dt;
            if (!manuallyTurnedOn) isFlashlightOn = false;
            
            if (Input()->Keyboard()->OnPressed(KeyboardKey::T)) 
            {
                isFlashlightOn = !isFlashlightOn;
                manuallyTurnedOn = isFlashlightOn;
            }
        }
        else if (!manuallyTurnedOn)
        {
            isFlashlightOn = true;
        }

        if (currentFlashlightCharge < 0.00001f)
        {
            currentFlashlightCharge = 0.0f;
            isFlashlightOn = false;
            manuallyTurnedOn = false;
        }
        if (Input()->Keyboard()->OnPressed(KeyboardKey::R))
        {
            currentFlashlightCharge = maxFlashlightCharge;
        }

        if (isFlashlightOn)
        {
            currentFlashlightCharge -= dt;

            flashlight->SetDamping(10.0f);
        }
        else
        {
            flashlight->SetDamping(1000.0f);
        }
        
        
        
        currentFlashlightCharge = std::min(std::max(0.0f, currentFlashlightCharge), maxFlashlightCharge);

        float progress = currentFlashlightCharge / maxFlashlightCharge;
        progressBar->GetComponent<cmp::Model>()->SetTintColor(progress, 0.0f, 0.06f);
    }

    void SetActiveIcon(int index)
    {
        for (int i = 0; i < 3; i++)
        {
            if (auto iconModel = iconsGO[i]->GetComponent<cmp::Model>())
            {
                if (i == index)
                {
                    iconModel->SetTintColor(activeColor);
                }
                else
                {
                    iconModel->SetTintColor(inactiveColor);
                }
            }
        }
    }

    void Unlock(int index)
    {
        iconsGO[index]->GetComponent<cmp::Transform>()->Move(0.0f, 0.0f, disabledIconsOffset);
    }

    bool IsFlashlightOn()
    {
        return isFlashlightOn;
    }

    int GetlightSourcesInRange()
    {
        return lightSourcesInRange;
    }
};