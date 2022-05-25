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


    //for public read
    bool isFlashlightOn = false;
    float maxFlashlightCharge = 10.0f; //10 seconds
    float currentFlashlightCharge;


public:

    void Start()
    {
        currentFlashlightCharge = maxFlashlightCharge;
    }

    void Update(float dt)
    {
        if (Input()->Keyboard()->OnPressed(KeyboardKey::T)) //obecnie tylko na przycisk, do zrobienia automatyczna aktywacja
        {
            isFlashlightOn = !isFlashlightOn;
        }

        if (isFlashlightOn)
        {
            currentFlashlightCharge -= dt;
            currentFlashlightCharge = std::max(0.0f, currentFlashlightCharge);
        }

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
};