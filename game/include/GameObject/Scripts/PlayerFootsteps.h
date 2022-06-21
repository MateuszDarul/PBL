#pragma once

#include "Components.h"
#include "SoundPlayer.h"

#include "PlayerGroundCheck.h"


class PlayerFootsteps : public Script
{
public:
    cmp::Camera* camera;
    PlayerGroundCheck* groundCheck;

private:

    SoundPlayer* footstepSFX;
    SoundPlayer* stairFootstepSFX;

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
        if (!stairFootstepSFX) stairFootstepSFX = new SoundPlayer("Resources/sounds/footstep_metal.wav");
    }

    ~PlayerFootsteps()
    {
        delete stairFootstepSFX;
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

                SoundPlayer* sound = footstepSFX;

                if (groundCheck->state == PlayerGroundCheck::State::STAIRS)
                    sound = stairFootstepSFX;

                sound->SetVolume(volume);
                sound->SetPitch(pitch);
                sound->Play();
            }
        }
        else
        {
            footstepTimer -= dt;
        }
    }
};