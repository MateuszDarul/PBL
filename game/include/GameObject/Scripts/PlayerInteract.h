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

    int ignoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::GUI | CollisionLayer::Ignore);


    //set these in 'inspector'

    CollidersManager* colMan;
    std::shared_ptr<cmp::Camera> camera;

    std::shared_ptr<cmp::Text> textTEMP;

    PlayerPlaceTurret* placeTurretScript;

private:

    MirrorRotate* selectedMirror = nullptr;

public:

    void Start()
    {

    }
    bool usingRMBToRotate = false;
    void Update(float dt)
    {
        bool canUseRMB = !placeTurretScript->isPlacing && Input()->Mouse()->OnPressed(MouseButton::Right_MB);
        bool shouldInteract = Input()->Keyboard()->OnPressed(KeyboardKey::E) || canUseRMB;


        placeTurretScript->isLookingAtMirror = false;
        textTEMP->color = { 1.0f, 0.0f, 0.0f };
        textTEMP->SetText("+");
        RayHitInfo hit;
        if (colMan->Raycast(camera->GetPosition(), camera->GetForward(), hit, interactRange, true, ignoreLayerMask))
        {
            auto scriptable = hit.gameObject->GetComponent<cmp::Scriptable>();
            if (!scriptable) scriptable = hit.gameObject->GetNode()->GetParent()->GetGameObject()->GetComponent<cmp::Scriptable>();
            if (scriptable)
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

                    if (!placeTurretScript->isPlacing && !selectedMirror)
                    {
                        placeTurretScript->isLookingAtMirror = true;
                        if (Input()->Mouse()->OnPressed(MouseButton::Left_MB))
                        {
                            selectedMirror = mirror;
                            selectedMirror->isPlayerInside = false;
                            selectedMirror->SetEnabled(true);
                            camera->SetRotationEnable(false);
                        }
                        else if (shouldInteract)
                        {
                            shouldInteract = false;
                            if (canUseRMB) usingRMBToRotate = true;

                            selectedMirror = mirror;
                            selectedMirror->isPlayerInside = true;
                            selectedMirror->SetEnabled(true);
                            camera->SetMovementEnable(false);
                        }
                    }

                    /*if (shouldInteract)
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
                    }*/
                }
                else if (auto turret = scriptable->Get<TurretLaser>())
                {
                    textTEMP->color = { 0.0f, 1.0f, 1.0f };

                    if (shouldInteract)
                    {
                        shouldInteract = false;
                        placeTurretScript->PickUpTurret(PlayerPlaceTurret::TurretType::Laser, hit.gameObject);
                    }

                }
                else if (auto turret = scriptable->Get<TurretShoot>())
                {
                    textTEMP->color = { 0.0f, 1.0f, 1.0f };
                    if (shouldInteract)
                    {
                        shouldInteract = false;
                        placeTurretScript->PickUpTurret(PlayerPlaceTurret::TurretType::Shooting, hit.gameObject);
                    }
                }
            }
        }

        
        

        bool kbpresd = kbpressed();



        if (selectedMirror && selectedMirror->isPlayerInside)
        {
            if ((usingRMBToRotate && Input()->Mouse()->IsReleased(MouseButton::Right_MB)) || shouldInteract)
            {
                usingRMBToRotate = false;
                selectedMirror->SetEnabled(false);
                selectedMirror = nullptr;
                camera->SetMovementEnable(true);
            }
        }

        if (selectedMirror && !selectedMirror->isPlayerInside)
        {
            //selectedMirror->disableRotation = Input()->Mouse()->IsPressed(MouseButton::Right_MB);
            //camera->SetRotationEnable(selectedMirror->disableRotation);
            
            const auto& camPos = camera->GetPosition();
            const auto& mirrorPos = selectedMirror->gameObject->GetComponent<cmp::Transform>()->GetPosition();
            const auto& posDiff = mirrorPos - camPos;
            float distanceSquared = glm::dot(posDiff, posDiff);
            
            if (Input()->Mouse()->IsReleased(MouseButton::Left_MB) || distanceSquared > 67.050f)
            {
                selectedMirror->SetEnabled(false);
                selectedMirror = nullptr;
                camera->SetRotationEnable(true);
                placeTurretScript->isLookingAtMirror = false;
            }
        }

        if (selectedMirror)
        {
            textTEMP->SetText("<>");
            placeTurretScript->isLookingAtMirror = true;
        }
    }

    bool kbpressed()
    {
        return Input()->Keyboard()->OnPressed(KeyboardKey::W) || Input()->Keyboard()->OnPressed(KeyboardKey::A) || Input()->Keyboard()->OnPressed(KeyboardKey::S) || Input()->Keyboard()->OnPressed(KeyboardKey::D);
    }
};