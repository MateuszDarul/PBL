#pragma once

#include "Components.h"
#include "SoundPlayer.h"


class PlayerFootsteps : public Script
{
public:
    cmp::Camera* camera;

private:

    SoundPlayer* footstepSFX;

    float footstepPeriod = 1.0f;
    float footstepTimer = -1.0f;

    float minVolume = 0.7f;
    float maxVolume = 1.0;
    float minPitch = 0.9f;
    float maxPitch = 1.3f;


public:

    void Start()
    {
        footstepPeriod = 3.14159265f / camera->GetSpeed();

        if (!footstepSFX) footstepSFX = new SoundPlayer("Resources/sounds/footstep.wav");
    }

    ~PlayerFootsteps()
    {
        delete footstepSFX;
    }

    void Update(float dt)
    {
        if (camera->IsMoving() && camera->GetIsGrounded())
        {
            if (footstepTimer > 0.0f)
            {
                footstepTimer -= dt;
            }
            else
            {
                footstepTimer = footstepPeriod;

                float volume = minVolume + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxVolume - minVolume)));
                float pitch = minPitch + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxPitch - minPitch)));

                footstepSFX->SetVolume(volume);
                footstepSFX->SetPitch(pitch);
                footstepSFX->Play();
            }
        }
        else
        {
            footstepTimer -= dt;
        }
    }
};