#pragma once

#include "Components.h"
#include "GameObject.h"

class DoorActivator : public Script 
{
public:

    //adjust these
    glm::vec3 openedOffset = { 0.0f, 6.0f, 0.0f };
    glm::vec3 brokenOffset = { 0.0f,3.0f,0.0f };
    float doorSpeed = 7.0f;

    //set these in 'inspector'

    std::shared_ptr<cmp::Transform> doorTransform;


private:

    std::shared_ptr<cmp::Model> buttonModel;

    bool isPrimed;
    bool isActivated;
    bool isShutdown;

    glm::vec3 targetPosition;
    
public:

    void Start()
    {
        isPrimed = true;
        isActivated = false;
        isShutdown = false;

        if (doorTransform) targetPosition = doorTransform->GetPosition();
        buttonModel = gameObject->GetComponent<cmp::Model>();
    }

    void Update(float dt)
    {
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

            if (doorTransform) targetPosition -= openedOffset;
            if (buttonModel) buttonModel->SetTintColor(0.88, 0.21, 0.21);
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
        if (!isShutdown)
        {
            if (isPrimed) //on power on
            {
                isPrimed = false;

                if (doorTransform) targetPosition += openedOffset;
                if (buttonModel) buttonModel->SetTintColor(0.21, 0.88, 0.21);
            }

            isActivated = true;
        }
    }

    void BrokenActivate()
    {
        if (!isShutdown)
        {
            if (isPrimed) //on power on
            {
                isPrimed = false;

                if (doorTransform) targetPosition += brokenOffset;
                if (buttonModel) buttonModel->SetTintColor({ 0.5f, 0.0f, 0.0f,  1.0f });
            }

        }
    }

    void ForceShutdown(bool shutdown = true)
    {
        isShutdown = shutdown;
        if (isShutdown) 
        { 
            if (doorTransform) targetPosition -= openedOffset;
            if (buttonModel) buttonModel->SetTintColor({ 0.5f, 0.0f, 0.0f,  1.0f });
        }
    }
};