#pragma once

#include "GameApplication.h"
#include "Components.h"

#include "GameManager.h"
#include "MultiToolController.h"
#include "TurretShoot.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

#include <iostream>


class PlayerPlaceTurret : public Script
{
public:

    //adjust these
    float placingRange = 5.0f;
    float shootingTurretRange = 15.0f;

    int ignoreLayerMask = ~(CollisionLayer::Player);


    //set these in 'inspector'

    GameManager* gameManager;
    MultiToolController* multiTool;

    Scene* scene;
    CollidersManager* colMan;
    ResourceManager* resMan;
    std::shared_ptr<cmp::Line> line;
    
    std::shared_ptr<cmp::Shader> turretShader; //err, should be better solved by a prefab or smth

    
    //for public READ

    bool isPlacing = false;


private: 

    std::shared_ptr<cmp::Transform> transform;
    std::shared_ptr<cmp::Camera> camera;

    std::shared_ptr<GameObject> turretToPlace;


public:

    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>();
        camera = gameObject->GetComponent<CameraComponent>();

        line->AddPoint(0, 0, 0);

        line->Set(0, {  0,  0,  0 });
        line->Set(1, {  1,  1, -2 });
        line->Set(2, {  1,  2, -3 });

        CreateTurret();
    }

    int turretType = -1;

    void Update(float dt)
    {
        bool needUpdate = false; 
        // dziesiec if'ow ale ¯\_(ツ)_/¯
        if (!isPlacing)
        {
            if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr1))
            {
                isPlacing = true;
                turretType = 0;
                needUpdate = true;
            }
            else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr2))
            {
                isPlacing = true;
                turretType = 1;
                needUpdate = true;
            }
            else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr3))
            {
                isPlacing = true;
                turretType = 2;
                needUpdate = true;
            }
        }
        else
        {
            if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr1))
            {
                if (turretType == 0) 
                {
                    isPlacing = false;
                    turretType = -1;
                }
                else
                {
                    turretType = 0;
                }
                needUpdate = true;
            }
            else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr2))
            {
                if (turretType == 1) 
                {
                    isPlacing = false;
                    turretType = -1;
                }
                else
                {
                    turretType = 1;
                }
                needUpdate = true;
            }
            else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr3))
            {
                if (turretType == 2) 
                {
                    isPlacing = false;
                    turretType = -1;
                }
                else
                {
                    turretType = 2;
                }
                needUpdate = true;
            }
        }
        if (needUpdate)
        {
            multiTool->SetActiveIcon(turretType);
        }

        int lineIndexToPlace = 2;

        if (isPlacing)
        {
            // !! hardcoded cost !!
            if (Input()->Mouse()->OnPressed(MouseButton::Left_MB) && gameManager->GetCurrentEnergy() > 80) 
            {
                turretToPlace->GetComponent<cmp::Scriptable>()->Get<TurretShoot>()->isPut = true;
                CreateTurret();
                gameManager->DescreaseEnergy(80);
            }

            RayHitInfo hit;
            if (colMan->Raycast(transform->GetPosition(), camera->GetForward(), hit, placingRange, false, ignoreLayerMask))
            {
                line->Set(1, hit.point - transform->GetPosition());
            }
            else
            {
                line->Set(1, camera->GetForward() * placingRange); 
            }            

            if (colMan->Raycast(transform->GetPosition() + line->Get(1), {0.0f, -1.0f, 0.0f}, hit, 10.0f, false, ignoreLayerMask))
            {
                line->Set(2, hit.point - transform->GetPosition());
            }
            else
            {
                line->Set(2, line->Get(1) + glm::vec3(0.0f, -1.0f, 0.0f));
                lineIndexToPlace = 1;
            }  
            
        }
        else
        line->Set(2, {0.0f, 999.9f, 0.0f}); //'delete' turret xd

        if (turretToPlace)
        {
            glm::vec3 adjust = {0.0f, 0.0f, 0.0f};
            turretToPlace->GetComponent<cmp::Transform>()->SetPosition(line->Get(lineIndexToPlace) + transform->GetPosition() + adjust);
            turretToPlace->GetComponent<cmp::Transform>()->SetRotation(0.0f, -camera->GetYaw(), 0.0f);
        }
    }

    void CreateTurret() //err, should be better solved by a prefab or smth
    {
        turretToPlace = std::make_shared<GameObject>();
    
        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj"),
            resMan->GetMaterial("Resources/models/wieze/w1/w1.mtl")
        );
        turretToPlace->AddComponent(mc);
        turretToPlace->AddComponent(turretShader);
        turretToPlace->AddComponent(std::make_shared<cmp::Transform>());
        turretToPlace->GetComponent<cmp::Transform>()->SetPosition(0,0.5,-5);
        turretToPlace->AddComponent(std::make_shared<cmp::FrustumCulling>());
        turretToPlace->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj"));
        turretToPlace->AddComponent(std::make_shared<cmp::SphereCol>(true, true));
        std::shared_ptr<cmp::SphereCol> col = turretToPlace->GetComponent<cmp::SphereCol>();
        col->SetRadius(shootingTurretRange);
        col->AddToCollidersManager(scene->GetCollidersManager());
        turretToPlace->AddComponent(std::make_shared<cmp::Scriptable>());
        TurretShoot* script = new TurretShoot();
        turretToPlace->GetComponent<cmp::Scriptable>()->Add(script);
    
        scene->AddGameObject(turretToPlace);
    }
};