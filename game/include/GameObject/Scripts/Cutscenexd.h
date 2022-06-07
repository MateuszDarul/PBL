#pragma once

#include "Components.h"
#include "DoorActivator.h"
#include "Health.h"
#include "ShadowsManager.h"


class Cutscenexd : public Script
{
public:

    DoorActivator* doorsToShut;
    DoorActivator* doorsToOpen;
    std::shared_ptr<cmp::Shader> lightShader;
    ShadowsManager* shadowManager;

    Health* enemyHealth;


private:
    std::shared_ptr<GameObject> alarmLightGO;
    std::shared_ptr<cmp::PointLight> lightCmp;


    bool hasStarted, hasFinished;
    
    bool isFadingOutAnimation = true;

public:

    void Start()
    {
        hasStarted = false;
        hasFinished = false;
    }

    void Update(float dt)
    {
        if (hasFinished)
        {
            doorsToOpen->Activate();
        }
        else if (hasStarted)
        {
            float current = lightCmp->GetLightColor().r;
            if (current < 0.11f) isFadingOutAnimation = false;
            else if (current > 0.999f) isFadingOutAnimation = true;

            if (isFadingOutAnimation) lightCmp->SetLightColor({current - dt, 0.0f, 0.0f});
            else lightCmp->SetLightColor({current + dt, 0.0f, 0.0f});

            if (enemyHealth)
            {
                if (enemyHealth->GetHealth() <= 0.0f)
                {
                    EndCutscene();
                }
            }
            else
            {
                EndCutscene();
            }
        }
    }

    void EndCutscene()
    {
        hasFinished = true;
        printf("Finished cutscene\n");

        lightCmp->SetLightColor({0.2f, 0.75f, 0.2f}); 
    }

    void TriggerEnter(std::shared_ptr<ColliderComponent> other) override
    {
        if (other->layer & CollisionLayer::Player && !hasStarted)
        {
            // Player entered trigger
            hasStarted = true;
            printf("Triggering cutscene...\n");

            doorsToShut->ForceShutdown();

            alarmLightGO = std::make_shared<GameObject>();
            gameObject->GetNode()->GetRoot()->FindNode("MAIN")->AddChild(alarmLightGO);
            lightCmp = std::make_shared<cmp::PointLight>();
            alarmLightGO->AddComponent(lightCmp);
            lightCmp->Create();
            lightCmp->AddShader(lightShader);
            lightCmp->SetPosition({-73.75, 4.0, 63.0});
            lightCmp->SetLightColor({0.2f, 0.0f, 0.0f});
            lightCmp->SetDamping(20.0f);

            shadowManager->AddLight(alarmLightGO.get());
        }
    }
};