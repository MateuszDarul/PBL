#pragma once

#include "GameApplication.h"
#include "Components.h"

#include "GameManager.h"
#include "MultiToolController.h"
#include "TurretLaser.h"
#include "TurretLight.h"
#include "TurretShoot.h"
#include "PlacedTurret.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

#include <iostream>


class PlayerPlaceTurret : public Script
{
public:

    //adjust these
    float placingRange = 5.0f;
    float shootingTurretRange = 15.0f;

    int ignoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::Ignore);

    enum TurretType {
        None = -1, Blockade = 0, Shooting = 1, Laser = 2
    };

    bool unlocked[3] = { false, false, false };
    TurretType selectedTurretType = TurretType::None;
    int turretCosts[3] = { 30, 80, 40 };


    //set these in 'inspector'

    GameManager* gameManager;
    MultiToolController* multiTool;

    SceneNode* turretsHolder;
    CollidersManager* colMan;
    ResourceManager* resMan;
    std::shared_ptr<cmp::Line> line;
    
    std::shared_ptr<cmp::Shader> turretShader; //err, should be better solved by a prefab or smth
    std::shared_ptr<cmp::Shader> lineShader;

    
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

        CreateTurret(TurretType::Blockade);
        CreateTurret(TurretType::Shooting);
        CreateTurret(TurretType::Laser);
    }

    void Update(float dt)
    {
        bool needUpdate = false; 
        // dziesiec if'ow ale ¯\_(ツ)_/¯
        if (!isPlacing)
        {
            if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr1) && unlocked[TurretType::Blockade])
            {
                isPlacing = true;
                selectedTurretType = TurretType::Blockade;
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
            if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr1)  && unlocked[TurretType::Blockade])
            {
                if (selectedTurretType == TurretType::Blockade)
                {
                    isPlacing = false;
                    selectedTurretType = TurretType::None;
                }
                else
                {
                    selectedTurretType = TurretType::Blockade;
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
            PlacedTurret* placedTurret = turretPrefabs[selectedTurretType]->GetComponent<cmp::Scriptable>()->Get<PlacedTurret>();
            if (Input()->Mouse()->OnPressed(MouseButton::Left_MB) && gameManager->GetCurrentEnergy() >= turretCosts[selectedTurretType])
            {
                if (auto scriptHolder = turretPrefabs[selectedTurretType]->GetComponent<cmp::Scriptable>())
                {
                    scriptHolder->EnableAll();
                    
                    /*std::shared_ptr<ColliderComponent> col = nullptr;
                    col = turretPrefabs[selectedTurretType]->GetComponent<cmp::SphereCol>();
                    if (!col) col = turretPrefabs[selectedTurretType]->GetComponent<cmp::BoxCol>();
                    if(col)
                    {
                        col->AddToCollidersManager(colMan);
                    }
                    */
                }
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

        //turretPrefabs[0]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);
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
        switch (type)
        {
        case TurretType::Blockade:
            CreateBlockadeTurret();
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
    }

    void CreateBlockadeTurret()
    {
    }

    void CreateShootingTurret()
    {
        TurretType type = TurretType::Shooting;
        turretPrefabs[type] = std::make_shared<GameObject>();

        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Wieze/Strzelajaca.obj"),
            resMan->GetMaterial("Resources/models/Wieze/Strzelajaca.mtl")
        );
        turretPrefabs[type]->AddComponent(mc);
        turretPrefabs[type]->AddComponent(turretShader);
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::Transform>());
        turretPrefabs[type]->GetComponent<cmp::Transform>()->SetPosition(0,0.5,-5);
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::FrustumCulling>());
        turretPrefabs[type]->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Wieze/Strzelajaca.obj"));
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::BoxCol>(true, false));
        std::shared_ptr<cmp::BoxCol> col2 = turretPrefabs[type]->GetComponent<cmp::BoxCol>();
        col2->SetLengths(glm::vec3(2.0f, 3.0f, 2.0f));
        col2->AddToCollidersManager(colMan);
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::Scriptable>());
        PlacedTurret* script2 = new PlacedTurret();
        turretPrefabs[type]->GetComponent<cmp::Scriptable>()->Add(script2);

        auto gfxGO = std::make_shared<GameObject>();
        gfxGO->AddComponent(std::make_shared<cmp::Transform>());
        gfxGO->AddComponent(std::make_shared<cmp::SphereCol>(true, false));
        std::shared_ptr<cmp::SphereCol> col = gfxGO->GetComponent<cmp::SphereCol>();
        col->SetRadius(shootingTurretRange);
        col->AddToCollidersManager(colMan);
        gfxGO->AddComponent(std::make_shared<cmp::Scriptable>());
        TurretShoot* script = new TurretShoot();
        script->isPut = true;
        gfxGO->GetComponent<cmp::Scriptable>()->Add(script);
        script->placedTurretScript = script2;

        turretsHolder->AddChild(turretPrefabs[type])->AddChild(gfxGO);
    }

    void CreateLaserTurret()
    {
        TurretType type = TurretType::Laser;
        turretPrefabs[type] = std::make_shared<GameObject>();
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::Transform>());


        auto scriptHolder = std::make_shared<cmp::Scriptable>();
        turretPrefabs[type]->AddComponent(scriptHolder);

        auto turretScript = new TurretLaser();
        turretScript->colMan = colMan;
        turretScript->SetEnabled(false);
        PlacedTurret* script2 = new PlacedTurret();
        turretPrefabs[type]->GetComponent<cmp::Scriptable>()->Add(script2);
        turretScript->placedTurretScript = script2;

        turretPrefabs[type]->AddComponent(std::make_shared<cmp::BoxCol>(true, false));
        std::shared_ptr<cmp::BoxCol> col = turretPrefabs[type]->GetComponent<cmp::BoxCol>();
        col->SetLengths(glm::vec3(2.0f, 3.0f, 2.0f));
        col->AddToCollidersManager(colMan);
        
        auto line = std::make_shared<cmp::Line>();
        line->Create();
        line->thickness = 2.0f;
        line->color1 = { 1.0f, 1.0f, 0.0f };
        line->color2 = { 1.0f, 0.7f, 0.0f };

        turretPrefabs[type]->AddComponent(line);
        turretPrefabs[type]->AddComponent(lineShader);
        turretScript->line = line.get();


        scriptHolder->Add(turretScript);
        


        auto gfxGO = std::make_shared<GameObject>();
        gfxGO->AddComponent(std::make_shared<cmp::Transform>());

        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Wieze/Laser.obj"),
            resMan->GetMaterial("Resources/models/Wieze/Laser.mtl")
        );
        gfxGO->AddComponent(mc);
        
        gfxGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        gfxGO->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Wieze/Laser.obj")
        );
        gfxGO->AddComponent(turretShader);


        turretsHolder->AddChild(turretPrefabs[type])->AddChild(gfxGO);
    }

};