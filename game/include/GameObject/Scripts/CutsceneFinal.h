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

    DoorActivator* doorsToOpen;
    DoorActivator* doorsToShut;
    EnemySpawnerScript* spawner;

    Cutscenexd* prevScene;

    bool hasStarted;
    bool hasFinished;

    bool spawned;

private:

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
            doorsToOpen->Activate();
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

            doorsToShut->ForceShutdown();
        }
    }
};