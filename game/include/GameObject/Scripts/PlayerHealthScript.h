#pragma once

#include "Components.h"
#include "GameObject.h"
#include "GameApplication.h"

class PlayerHealthScript : public Script
{
public:

    bool inLight, attacked;

    float playerHealth;

    std::shared_ptr<cmp::Camera> player;

    MultiToolController* multiTool;

    void Start()
    {
        playerHealth = 100.0f;
        inLight = true;
        attacked = false;

        multiTool = gameObject->GetNode()->GetParent()->GetParent()->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<MultiToolController>();
    }

    void Update(float dt)
    {
        GameApplication::SetContrastMod((100.0f - playerHealth) * 0.01f * 2.0f);

        if (playerHealth > 100.0f)
        {
            playerHealth = 100.0f;
        }

        if (playerHealth < 0)
        {
            GameApplication::LoadSceneLevel();
        }

        if (attacked)
        {
            playerHealth -= 50.0f * dt;
        }

        if (multiTool->GetlightSourcesInRange() > 0)
        {
            playerHealth += 30.0f * dt;
        }
        else if (multiTool->IsFlashlightOn() == false)
        {
            playerHealth -= 10.0f * dt;
        }
    }

    void TriggerEnter(std::shared_ptr<ColliderComponent> other) override
    {
        if (other->layer == ENEMY)
        {
            attacked = true;
        }
    }

    void TriggerExit(std::shared_ptr<ColliderComponent> other) override
    {
        if (other->layer == ENEMY)
        {
            attacked = false;
        }
    }
};