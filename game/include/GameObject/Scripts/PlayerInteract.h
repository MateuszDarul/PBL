#pragma once

#include "GameApplication.h"
#include "Components.h"

#include "Resource.h"


class PlayerInteract : public Script
{
public:

    //adjust these

    float interactRange = 3.5f;

    int ignoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::GUI);


    //set these in 'inspector'

    CollidersManager* colMan;
    std::shared_ptr<cmp::Camera> camera;

    std::shared_ptr<cmp::Text> textTEMP;


public:

    void Start()
    {
        
    }

    void Update(float dt)
    {
        bool shouldInteract = false;
        if (Input()->Keyboard()->OnPressed(KeyboardKey::E))
        {
            shouldInteract = true;
        }

        textTEMP->color = { 1.0f, 0.0f, 0.0f };
        RayHitInfo hit;
        if (colMan->Raycast(camera->GetPosition(), camera->GetForward(), hit, interactRange, true, ignoreLayerMask))
        {
            if (auto scriptable = hit.gameObject->GetComponent<cmp::Scriptable>())
            {
                if (auto resource = scriptable->Get<Resource>())
                {
                    textTEMP->color = { 0.0f, 1.0f, 0.0f };
                    if (shouldInteract)
                    {
                        printf("Picked resource\n");
                        resource->PickUp();
                    }
                }
            }
        }
    }
};