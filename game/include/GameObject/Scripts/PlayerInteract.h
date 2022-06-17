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
    float isLookingAtMirrorDotThreshold = 0.81f;

    int ignoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::GUI | CollisionLayer::Ignore);


    //set these in 'inspector'

    CollidersManager* colMan;
    std::shared_ptr<cmp::Camera> camera;

    std::shared_ptr<cmp::Text> textTEMP;

    PlayerPlaceTurret* placeTurretScript;

    //only for READ
    MirrorRotate* selectedMirror = nullptr;

private:

    GameObject* tooltip;
    GameObject* mirrorControlsText;

    bool usingRMBToRotate = false;
    bool isCurrentlyRotatingWASD = false;
    float mirrorRotInversion = 1.0f;


public:

    void Start()
    {
        tooltip = gameObject->GetNode()->GetRoot()->FindNode("Tooltip")->GetGameObject().get();
        mirrorControlsText = gameObject->GetNode()->GetRoot()->FindNode("MirrorControlsText")->GetGameObject().get();
    }

    void Update(float dt)
    {
        bool canUseRMB = !placeTurretScript->isPlacing && Input()->Mouse()->OnPressed(MouseButton::Right_MB);
        bool shouldInteract = Input()->Keyboard()->OnPressed(KeyboardKey::E) || canUseRMB;

        bool showTooltip = false;
        tooltip->GetComponent<TransformComponent>()->SetScale(0.0f);
        mirrorControlsText->GetComponent<TransformComponent>()->SetScale(0.0f);

        placeTurretScript->isLookingAtMirror = false;
        textTEMP->color = { 1.0f, 0.0f, 0.0f };
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
                    showTooltip = true;
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
                    showTooltip = true;
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
                    showTooltip = true;
                    textTEMP->color = { 0.0f, 1.0f, 0.0f };

                    if (!placeTurretScript->isPlacing && !selectedMirror)
                    {
                        placeTurretScript->isLookingAtMirror = true;
                        //if (false && Input()->Mouse()->OnPressed(MouseButton::Left_MB))
                        //{ // lmb only - currently unused
                        //    selectedMirror = mirror;
                        //    selectedMirror->disableMouseRotation = false;
                        //    selectedMirror->SetEnabled(true);
                        //    camera->SetRotationEnable(false);
                        //}
                        //else 
                        if (shouldInteract)
                        {
                            shouldInteract = false;

                            selectedMirror = mirror;
                            selectedMirror->SetEnabled(true);
                            camera->SetMovementEnable(false);

                            selectedMirror->disableKeyboardRotation = true;
                            selectedMirror->disableMouseRotation = true;
                            
                            if (canUseRMB) usingRMBToRotate = true;
                        }
                    }
                }
                else if (auto turret = scriptable->Get<TurretLaser>())
                {
                    showTooltip = true;
                    textTEMP->color = { 0.0f, 1.0f, 1.0f };

                    if (shouldInteract)
                    {
                        shouldInteract = false;
                        placeTurretScript->PickUpTurret(PlayerPlaceTurret::TurretType::Laser, hit.gameObject);
                    }

                }
                else if (auto turret = scriptable->Get<TurretShoot>())
                {
                    showTooltip = true;
                    textTEMP->color = { 0.0f, 1.0f, 1.0f };
                    if (shouldInteract)
                    {
                        shouldInteract = false;
                        placeTurretScript->PickUpTurret(PlayerPlaceTurret::TurretType::Shooting, hit.gameObject);
                    }
                }
            }
        }

        if (showTooltip)
        {
            tooltip->GetComponent<TransformComponent>()->SetScale(0.2f);
        }

        bool isNewWASDInput = onwasdpressed();
        isCurrentlyRotatingWASD = selectedMirror && iswasdpressed();

        if (selectedMirror)
        {
            textTEMP->SetText("<>");
            placeTurretScript->isLookingAtMirror = true;
            mirrorControlsText->GetComponent<TransformComponent>()->SetScale(0.07f);

            //if (usingRMBToRotate)
            //{
                bool isLmbReleased = Input()->Mouse()->IsReleased(MouseButton::Left_MB);
                selectedMirror->disableKeyboardRotation = !isNewWASDInput && selectedMirror->disableKeyboardRotation;

                if (!isCurrentlyRotatingWASD)
                {
                    const auto& mirrorPos = selectedMirror->gameObject->GetComponent<cmp::Transform>()->GetPosition();
                    bool isLookingAtMirror = glm::dot(camera->GetForward(), glm::normalize(mirrorPos - camera->GetPosition())) > isLookingAtMirrorDotThreshold;
                    mirrorRotInversion = (isLookingAtMirror) ? 1.0f : -1.0f;
                }

                // when looking away (inverted rotation) - cursor red; when no inverted rotation - green
                textTEMP->color = (mirrorRotInversion < 0 ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f));
                
                selectedMirror->disableMouseRotation = isLmbReleased;
                selectedMirror->invertRotationY_temp = mirrorRotInversion;
                camera->SetRotationEnable(isLmbReleased);
                if (usingRMBToRotate && Input()->Mouse()->IsReleased(MouseButton::Right_MB))
                {
                    usingRMBToRotate = false;
                    selectedMirror->SetEnabled(false);
                    selectedMirror = nullptr;
                    camera->SetMovementEnable(true);
                }
            //}
            //else if (!selectedMirror->disableMouseRotation)
            //{ // lmb only - currently unused
            //    const auto& camPos = camera->GetPosition();
            //    const auto& mirrorPos = selectedMirror->gameObject->GetComponent<cmp::Transform>()->GetPosition();
            //    const auto& posDiff = mirrorPos - camPos;
            //    float distanceSquared = glm::dot(posDiff, posDiff);
            //
            //    if (Input()->Mouse()->IsReleased(MouseButton::Left_MB) || distanceSquared > 67.050f)
            //    {
            //        selectedMirror->SetEnabled(false);
            //        selectedMirror = nullptr;
            //        camera->SetRotationEnable(true);
            //        placeTurretScript->isLookingAtMirror = false;
            //    }
            //}
            if (shouldInteract)
            {
                usingRMBToRotate = false;
                selectedMirror->SetEnabled(false);
                selectedMirror = nullptr;
                camera->SetMovementEnable(true);
                camera->SetRotationEnable(true);
            }
        }
    }

    bool onwasdpressed()
    {
        return Input()->Keyboard()->OnPressed(KeyboardKey::W) || Input()->Keyboard()->OnPressed(KeyboardKey::A) || Input()->Keyboard()->OnPressed(KeyboardKey::S) || Input()->Keyboard()->OnPressed(KeyboardKey::D);
    }

    bool iswasdpressed()
    {
        return Input()->Keyboard()->IsPressed(KeyboardKey::W) || Input()->Keyboard()->IsPressed(KeyboardKey::A) || Input()->Keyboard()->IsPressed(KeyboardKey::S) || Input()->Keyboard()->IsPressed(KeyboardKey::D);
    }
};