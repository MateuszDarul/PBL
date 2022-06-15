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
    GameObject* tooltip;

public:

    void Start()
    {
        tooltip = gameObject->GetNode()->GetRoot()->FindNode("Tooltip")->GetGameObject().get();
    }
    bool usingRMBToRotate = false;
    glm::vec3 saveCamPos;
    glm::vec2 saveCamRot;
    void Update(float dt)
    {
        bool canUseRMB = !placeTurretScript->isPlacing && Input()->Mouse()->OnPressed(MouseButton::Right_MB);
        bool shouldInteract = Input()->Keyboard()->OnPressed(KeyboardKey::E) || canUseRMB;


        placeTurretScript->isLookingAtMirror = false;
        textTEMP->color = { 1.0f, 0.0f, 0.0f };
        tooltip->GetComponent<TransformComponent>()->SetScale(0.0f);
        textTEMP->SetText("+");
        RayHitInfo hit;
        if (colMan->Raycast(camera->GetPosition(), camera->GetForward(), hit, interactRange, true, ignoreLayerMask))
        {
            auto scriptable = hit.gameObject->GetComponent<cmp::Scriptable>();
            if (!scriptable) scriptable = hit.gameObject->GetNode()->GetParent()->GetGameObject()->GetComponent<cmp::Scriptable>();
            if (scriptable && !selectedMirror)
            {
                if (auto resource = scriptable->Get<Resource>())
                {
                    textTEMP->color = { 0.0f, 1.0f, 0.0f };
                    placeTurretScript->isLookingAtMirror = true; // to disable player placing
                    if (shouldInteract || Input()->Mouse()->OnPressed(MouseButton::Left_MB))
                    {
                        shouldInteract = false;
                        printf("Picked resource\n");
                        resource->PickUp();
                    }
                }
                else if (auto blueprint = scriptable->Get<Blueprint>())
                {
                    tooltip->GetComponent<TransformComponent>()->SetScale(0.1f);
                    textTEMP->color = { 0.0f, 1.0f, 0.0f };
                    placeTurretScript->isLookingAtMirror = true; // to disable player placing
                    if (shouldInteract || Input()->Mouse()->OnPressed(MouseButton::Left_MB))
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
                            selectedMirror->disableMouseRotation = false;
                            selectedMirror->SetEnabled(true);
                            camera->SetRotationEnable(false);
                        }
                        else if (shouldInteract)
                        {
                            shouldInteract = false;

                            selectedMirror = mirror;
                            selectedMirror->SetEnabled(true);
                            camera->SetMovementEnable(false);

                            selectedMirror->isPlayerInside = false;
                            selectedMirror->disableMouseRotation = true;
                            
                            if (canUseRMB) usingRMBToRotate = true;
                        }
                    }
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


        if (selectedMirror)
        {
            textTEMP->SetText("<>");
            placeTurretScript->isLookingAtMirror = true;

            if (usingRMBToRotate)
            {
                bool isLmbReleased = Input()->Mouse()->IsReleased(MouseButton::Left_MB);
                selectedMirror->disableMouseRotation = isLmbReleased;
                selectedMirror->invertRotationY_temp = (isLmbReleased) ? 1.0f : -1.0f;
                camera->SetRotationEnable(isLmbReleased);
                if (Input()->Mouse()->IsReleased(MouseButton::Right_MB))
                {
                    usingRMBToRotate = false;
                    selectedMirror->SetEnabled(false);
                    selectedMirror = nullptr;
                    camera->SetMovementEnable(true);
                }
            }
            else if (!selectedMirror->disableMouseRotation)
            {
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
            else if (shouldInteract)
            {
                usingRMBToRotate = false;
                selectedMirror->SetEnabled(false);
                selectedMirror = nullptr;
                camera->SetMovementEnable(true);
            }
        }
    }

    bool kbpressed()
    {
        return Input()->Keyboard()->OnPressed(KeyboardKey::W) || Input()->Keyboard()->OnPressed(KeyboardKey::A) || Input()->Keyboard()->OnPressed(KeyboardKey::S) || Input()->Keyboard()->OnPressed(KeyboardKey::D);
    }
};