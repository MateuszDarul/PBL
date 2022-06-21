#pragma once

#include "Components.h"
#include "GameObject.h"

class DoorActivator : public Script 
{
public:

    //adjust these
    glm::vec3 openedOffset = { 0.0f, 6.0f, 0.0f };
    float doorSpeed = 7.0f;

    //set these in 'inspector'

    std::shared_ptr<cmp::Transform> doorTransform;

    enum class State
    {
        ACTIVE, INACTIVE, SHUTDOWN
    };

    State state;

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
        state = State::INACTIVE;

        if (doorTransform) targetPosition = doorTransform->GetPosition();
        buttonModel = gameObject->GetComponent<cmp::Model>();
    }

    void Update(float dt)
    {
        if (isShutdown) 
        {
            isActivated = false;
            isPrimed = true;
            state = State::SHUTDOWN;
        }
        else if (isActivated)    //on being powered
        {
            isActivated = false;
            state = State::ACTIVE;
            
        }
        else if (!isPrimed)  //on power off
        {
            isPrimed = true;
            state = State::INACTIVE;

            if (doorTransform) targetPosition -= openedOffset;
            if (buttonModel) buttonModel->SetTintColor(0.88, 0.21, 0.21);
        }
        else                //on being unpowered
        {
            state = State::INACTIVE;
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
                state = State::ACTIVE;

                if (doorTransform) targetPosition += openedOffset;
                if (buttonModel) buttonModel->SetTintColor(0.21, 0.88, 0.21);
            }

            isActivated = true;
        }
    }

    void ForceShutdown(bool shutdown = true)
    {
        isShutdown = shutdown;
        if (isShutdown) 
        { 
            state = State::SHUTDOWN;
            if (doorTransform) targetPosition -= openedOffset;
            if (buttonModel) buttonModel->SetTintColor({ 0.5f, 0.0f, 0.0f,  1.0f });
        }
    }
};