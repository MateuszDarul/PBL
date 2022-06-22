#pragma once

#include "Components.h"
#include "AudioManager.h"


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
    float maxFlashlightCharge = 15.0f; //15 seconds
    float currentFlashlightCharge;

    //
    int lightSourcesInRange = 0;

private:

    float disabledIconsOffset = 0.1f;
    bool manuallyTurnedOn = false;
    bool turnedOffMusic = false;

public:

    void Start()
    {
        currentFlashlightCharge = maxFlashlightCharge;

        for (int i = 0; i < 3; i++)
        {
            // 'disabling' icons
            Lock(i);
        }
    }
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
            if (isFlashlightOn)
            {
                AudioManager::FadeOutMusic(1.0f);
                turnedOffMusic = true;
            }

            currentFlashlightCharge = 0.0f;
            isFlashlightOn = false;
            manuallyTurnedOn = false;
        }
        else if (turnedOffMusic && (currentFlashlightCharge > 0.5f * maxFlashlightCharge))
        {
            turnedOffMusic = false;
            AudioManager::FadeInMusic(2.0f);
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
        auto pos = iconsGO[index]->GetComponent<cmp::Transform>()->GetPosition();
        pos.z = 0.0f;
        iconsGO[index]->GetComponent<cmp::Transform>()->SetPosition(pos);
    }

    void Lock(int index)
    {
        auto pos = iconsGO[index]->GetComponent<cmp::Transform>()->GetPosition();
        pos.z = -disabledIconsOffset;
        iconsGO[index]->GetComponent<cmp::Transform>()->SetPosition(pos);
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