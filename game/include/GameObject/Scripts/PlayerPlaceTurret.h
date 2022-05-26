#pragma once

#include "GameApplication.h"
#include "Components.h"

#include "GameManager.h"
#include "MultiToolController.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

#include <iostream>


class PlayerPlaceTurret : public Script
{
public:

    //adjust these
    float placingRange = 5.0f;

    int ignoreLayerMask = ~(CollisionLayer::Player);

    enum TurretType {
        None = -1, Lantern = 0, Shooting = 1, Laser = 2
    };

    bool unlocked[3] = { false, false, false };
    TurretType selectedTurretType = TurretType::None;
    int turretCosts[3] = { 80, 100, 60 };


    //set these in 'inspector'

    GameManager* gameManager;
    MultiToolController* multiTool;

    SceneNode* turretsHolder;
    CollidersManager* colMan;
    ResourceManager* resMan;
    std::shared_ptr<cmp::Line> line;
    
    std::shared_ptr<cmp::Shader> turretShader; //err, should be better solved by a prefab or smth

    
    //for public READ

    bool isPlacing = false;


private: 

    std::shared_ptr<cmp::Transform> transform;
    std::shared_ptr<cmp::Camera> camera;

    std::shared_ptr<GameObject> turretPrefabs[3];


public:

    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>();
        camera = gameObject->GetComponent<CameraComponent>();

        line->AddPoint(0, 0, 0);

        line->Set(0, {  0,  0,  0 });
        line->Set(1, {  1,  1, -2 });
        line->Set(2, {  1,  2, -3 });

        CreateTurret(TurretType::Lantern);
        CreateTurret(TurretType::Shooting);
        CreateTurret(TurretType::Laser);
    }

    void Update(float dt)
    {
        bool needUpdate = false; 
        // dziesiec if'ow ale ¯\_(ツ)_/¯
        if (!isPlacing)
        {
            if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr1) && unlocked[TurretType::Lantern])
            {
                isPlacing = true;
                selectedTurretType = TurretType::Lantern;
                needUpdate = true;
            }
            else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr2) && unlocked[TurretType::Shooting])
            {
                isPlacing = true;
                selectedTurretType = TurretType::Shooting;
                needUpdate = true;
            }
            else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr3)  && unlocked[TurretType::Laser])
            {
                isPlacing = true;
                selectedTurretType = TurretType::Laser;
                needUpdate = true;
            }
        }
        else
        {
            if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr1)  && unlocked[TurretType::Lantern])
            {
                if (selectedTurretType == TurretType::Lantern) 
                {
                    isPlacing = false;
                    selectedTurretType = TurretType::None;
                }
                else
                {
                    selectedTurretType = TurretType::Lantern;
                }
                needUpdate = true;
            }
            else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr2) && unlocked[TurretType::Shooting])
            {
                if (selectedTurretType == TurretType::Shooting) 
                {
                    isPlacing = false;
                    selectedTurretType = TurretType::None;
                }
                else
                {
                    selectedTurretType = TurretType::Shooting;
                }
                needUpdate = true;
            }
            else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr3) && unlocked[TurretType::Laser])
            {
                if (selectedTurretType == TurretType::Laser) 
                {
                    isPlacing = false;
                    selectedTurretType = TurretType::None;
                }
                else
                {
                    selectedTurretType = TurretType::Laser;
                }
                needUpdate = true;
            }
        }
        if (needUpdate)
        {
            multiTool->SetActiveIcon(selectedTurretType);
        }

        int lineIndexToPlace = 2;

        if (isPlacing)
        {
            if (Input()->Mouse()->OnPressed(MouseButton::Left_MB) && gameManager->GetCurrentEnergy() >= turretCosts[selectedTurretType]) 
            {
                CreateTurret(selectedTurretType);
                gameManager->DescreaseEnergy(turretCosts[selectedTurretType]);
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

        turretPrefabs[0]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);
        turretPrefabs[1]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);
        turretPrefabs[2]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);

        if (selectedTurretType != TurretType::None)
        {
            glm::vec3 adjust = {0.0f, 0.0f, 0.0f};
            turretPrefabs[selectedTurretType]->GetComponent<cmp::Transform>()->SetPosition(line->Get(lineIndexToPlace) + transform->GetPosition() + adjust);
            turretPrefabs[selectedTurretType]->GetComponent<cmp::Transform>()->SetRotation(0.0f, -camera->GetYaw(), 0.0f);
        }
    }

    void CreateTurret(TurretType type)
    {
        turretPrefabs[type] = std::make_shared<GameObject>();
        turretPrefabs[type]->AddComponent(turretShader);
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::Transform>());
        turretPrefabs[type]->GetComponent<cmp::Transform>()->SetPosition(0,0.5,-5);

        switch (type)
        {
        case TurretType::Lantern:
            CreateLanternTurret();
            break;
        case TurretType::Shooting:
            CreateShootingTurret();
            break;
        case TurretType::Laser:
            CreateLaserTurret();
            break;
        default:
            break;
        }

        turretsHolder->AddChild(turretPrefabs[type]);
    }

    void CreateLanternTurret()
    {
        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/wieze/w2/w2.obj"),
            resMan->GetMaterial("Resources/models/wieze/w2/w2.mtl")
        );
        turretPrefabs[TurretType::Lantern]->AddComponent(mc);
        
        turretPrefabs[TurretType::Lantern]->AddComponent(std::make_shared<cmp::FrustumCulling>());
        turretPrefabs[TurretType::Lantern]->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/wieze/w2/w2.obj")
        );
    }

    void CreateShootingTurret()
    {
        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj"),
            resMan->GetMaterial("Resources/models/wieze/w1/w1.mtl")
        );
        turretPrefabs[TurretType::Shooting]->AddComponent(mc);
        
        turretPrefabs[TurretType::Shooting]->AddComponent(std::make_shared<cmp::FrustumCulling>());
        turretPrefabs[TurretType::Shooting]->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj")
        );
    }

    void CreateLaserTurret()
    {
        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/wieze/w3/w3.obj"),
            resMan->GetMaterial("Resources/models/wieze/w3/w3.mtl")
        );
        turretPrefabs[TurretType::Laser]->AddComponent(mc);
        
        turretPrefabs[TurretType::Laser]->AddComponent(std::make_shared<cmp::FrustumCulling>());
        turretPrefabs[TurretType::Laser]->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/wieze/w3/w3.obj")
        );
    }

};