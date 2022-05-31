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

    glm::vec3 targetPosition;
private:

    std::shared_ptr<cmp::Model> buttonModel;

    bool isPrimed;
    bool isActivated;

public:

    void Start()
    {
        isPrimed = true;
        isActivated = false;

        if (doorTransform) targetPosition = doorTransform->GetPosition();
        buttonModel = gameObject->GetComponent<cmp::Model>();
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
            if (buttonModel) buttonModel->SetTintColor({ 1.0f, 1.0f, 1.0f,  1.0f });
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
            if (buttonModel) buttonModel->SetTintColor({ 0.2f, 1.0f, 0.2f,  1.0f });
        }

        isActivated = true;
    }
};