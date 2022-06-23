#pragma once

#include "Components.h"
#include "DoorActivator.h"
#include "Health.h"
#include "ShadowsManager.h"
#include "SoundPlayer.h"
#include "EnemySpawnerScript.h"
#include "Scripts/Cutscenexd.h"

class CutsceneFinal : public Script
{
public:

    DoorActivator* objectiveDoor;
    DoorActivator* doorsToOpen;
    std::shared_ptr<GameObject> doorsToShut;
    EnemySpawnerScript* spawner;

    Cutscenexd* prevScene;

    bool hasStarted;
    bool hasFinished;

    bool spawned;

private:

    float elevatorDoorTimer = 0.78f;

public:

    void Start()
    {
        hasStarted = false;
        hasFinished = false;
        spawned = false;
    }

    void Update(float dt)
    {
        if (hasFinished)
        {
            if (elevatorDoorTimer > 0.0f) elevatorDoorTimer -= dt;
            else doorsToOpen->Activate();
        }
        else if (hasStarted)
        {
            if (!spawned) {
                //yeah...
                spawner->SpawnEnemy(1);
                spawner->SpawnEnemy(1);
                spawned = true;
                prevScene->turnOffLight();
            }

            if (objectiveDoor && objectiveDoor->state == DoorActivator::State::ACTIVE)
            {
                EndCutscene();
            }
        }
    }

    void EndCutscene()
    {
        hasFinished = true;
    }

    void TriggerEnter(std::shared_ptr<ColliderComponent> other) override
    {
        if (other->layer & CollisionLayer::Player && !hasStarted)
        {
            // Player entered trigger
            hasStarted = true;

            doorsToShut->GetComponent<cmp::Transform>()->Move(0, -6, 0);
        }
    }
};