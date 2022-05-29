#pragma once

#include "GameApplication.h"
#include "Components.h"

#include "Resource.h"
#include "Blueprint.h"
#include "MirrorRotate.h"


class PlayerInteract : public Script
{
public:

    //adjust these

    float interactRange = 4.0f;

    int ignoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::GUI);


    //set these in 'inspector'

    CollidersManager* colMan;
    std::shared_ptr<cmp::Camera> camera;

    std::shared_ptr<cmp::Text> textTEMP;

private:

    MirrorRotate* selectedMirror = nullptr;

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
        textTEMP->SetText("+");
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
                        shouldInteract = false;
                        printf("Picked resource\n");
                        resource->PickUp();
                    }
                }
                else if (auto blueprint = scriptable->Get<Blueprint>())
                {
                    textTEMP->color = { 0.0f, 1.0f, 0.0f };
                    if (shouldInteract)
                    {
                        shouldInteract = false;
                        printf("Picked blueprint\n");
                        blueprint->PickUp();
                    }
                }
                else if (auto mirror = scriptable->Get<MirrorRotate>())
                {
                    textTEMP->color = { 0.0f, 1.0f, 0.0f };
                    if (shouldInteract)
                    {
                        shouldInteract = false;

                        if (selectedMirror)
                        {
                            if (selectedMirror == mirror)
                            {
                                camera->SetMovementEnable(true);
                                selectedMirror->SetEnabled(false);
                                selectedMirror = nullptr;
                            }
                            else
                            {
                                selectedMirror->SetEnabled(false);
                                selectedMirror = mirror;
                                selectedMirror->SetEnabled(true);
                            }
                        }
                        else
                        {
                            camera->SetMovementEnable(false);
                            selectedMirror = mirror;
                            selectedMirror->SetEnabled(true);
                        }

                        // if (selectedMirror) selectedMirror->SetEnabled(false);
                        // else camera->SetMovementEnable(!camera->GetMovementEnabled());


                        // selectedMirror = mirror;
                        // selectedMirror->SetEnabled(!camera->GetMovementEnabled());
                    }
                }
            }
        }

        if (shouldInteract)
        {
            // pressed Interact button and no interactable was found
            
            // removing focus from selected mirror - if any
            if (selectedMirror) selectedMirror->SetEnabled(false);
            selectedMirror = nullptr;
            camera->SetMovementEnable(true);
        }

        if (selectedMirror)
        {
            textTEMP->SetText("<>");
        }
    }
};