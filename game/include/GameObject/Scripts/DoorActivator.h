#pragma once

#include "Components.h"
#include "GameObject.h"
#include "SoundPlayer.h"
#include "SceneNode.h"

class DoorActivator : public Script 
{
public:

    //adjust these
    glm::vec3 openedOffset = { 0.0f, 6.0f, 0.0f };
    float timeToOpen  = 1.2f;
    float timeToClose = 0.2f;

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
    glm::vec3 originalPosition;

    SoundPlayer* beepON;
    SoundPlayer* beepOFF;
    SoundPlayer* doorOpenSFX;
    SoundPlayer* doorCloseSFX;
    bool canPlayOpenSound = true;
    bool canPlayCloseSound = true;
    bool fixFirstSound = false;

    float cooldownOpen = 0.3f;
    float cooldownClose = 0.1f;
    float cooldownTimer = -1.0f;
    float movingTimer = -1.0f;

    float doorSpeedOpening = 1.0f;
    float doorSpeedClosing = 1.0f;

public:

    void Start()
    {
        isPrimed = true;
        isActivated = false;
        isShutdown = false;
        state = State::INACTIVE;

        doorSpeedOpening = 1.0f / timeToOpen;
        doorSpeedClosing = 1.0f / timeToClose;

        if (doorTransform) targetPosition = doorTransform->GetPosition();
        originalPosition = targetPosition;

        buttonModel = gameObject->GetComponent<cmp::Model>();

        if (!beepON ) beepON  = new SoundPlayer("Resources/sounds/beep_on.wav");
        if (!beepOFF) beepOFF = new SoundPlayer("Resources/sounds/beep_off.wav");
        if (!doorOpenSFX) doorOpenSFX = new SoundPlayer("Resources/sounds/dooropen.wav");
        if (!doorCloseSFX) doorCloseSFX = new SoundPlayer("Resources/sounds/doorclose.wav");
    }

    ~DoorActivator()
    {
        delete doorCloseSFX;
        delete doorOpenSFX;
        delete beepOFF;
        delete beepON;
    }

    void Update(float dt)
    {
        if (isShutdown) 
        {
            isActivated = false;
            isPrimed = true;
            state = State::SHUTDOWN;

            if (doorTransform)
            {
                if (canPlayCloseSound && fixFirstSound)
                {
                    canPlayCloseSound = false;
                    canPlayOpenSound = true;
                    doorCloseSFX->Play3D(
                        originalPosition,
                        gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>().get());
                }

                glm::vec3 move = targetPosition - doorTransform->GetPosition();
                doorTransform->Move(move * doorSpeedClosing * dt);
            }
        }
        else if (isActivated)    //on being powered
        {
            isActivated = false;
            state = State::ACTIVE;
            
            if (cooldownTimer > 0.0f) cooldownTimer -= dt;
            else if (doorTransform)
            {
                if (canPlayOpenSound && fixFirstSound)
                {
                    canPlayOpenSound = false;
                    canPlayCloseSound = true;
                    doorOpenSFX->Play3D(
                        originalPosition,
                        gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>().get());
                }

                glm::vec3 move = targetPosition - doorTransform->GetPosition();
                doorTransform->Move(move * doorSpeedOpening * dt);
            }
        }
        else if (!isPrimed)  //on power off
        {
            isPrimed = true;
            state = State::INACTIVE;
            beepOFF->Play3D(
                gameObject->GetNode()->GetGlobalPosition(), 
                gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>().get());

            cooldownTimer = cooldownClose;
            fixFirstSound = true;

            targetPosition = originalPosition;
            if (buttonModel) buttonModel->SetTintColor(0.88, 0.21, 0.21);
        }
        else                //on being unpowered
        {
            state = State::INACTIVE;

            if (cooldownTimer > 0.0f) cooldownTimer -= dt;
            else if (doorTransform)
            {
                if (canPlayCloseSound && fixFirstSound)
                {
                    canPlayCloseSound = false;
                    canPlayOpenSound = true;
                    doorOpenSFX->Play3D(
                        originalPosition,
                        gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>().get());
                }

                glm::vec3 move = targetPosition - doorTransform->GetPosition();
                doorTransform->Move(move * doorSpeedClosing * dt);
            }
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
                beepON->Play3D(
                    gameObject->GetNode()->GetGlobalPosition(),
                    gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>().get());

                cooldownTimer = cooldownOpen;
                fixFirstSound = true;

                targetPosition = originalPosition + openedOffset;
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
            fixFirstSound = true;
            state = State::SHUTDOWN;
            targetPosition = originalPosition;
            if (buttonModel) buttonModel->SetTintColor({ 0.5f, 0.0f, 0.0f,  1.0f });
        }
    }
};