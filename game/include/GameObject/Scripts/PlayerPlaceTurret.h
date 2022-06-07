#pragma once

#include "GameApplication.h"
#include "Components.h"

#include "GameManager.h"
#include "MultiToolController.h"
#include "TurretLaser.h"
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

    int ignoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::GUI | CollisionLayer::Ignore);

    enum TurretType {
        None = -1, Laser = 0, Shooting = 1, Lantern = 2
    };

    bool unlocked[3] = { false, false, false };
    TurretType selectedTurretType = TurretType::None;
    int turretCosts[3] = { 30, 80, 40 };


    glm::vec4 turretGhostColor = { 0.0f, 1.0f, 0.0f,   1.0f };


    //set these in 'inspector'

    GameManager* gameManager;
    MultiToolController* multiTool;

    SceneNode* turretsHolder;
    CollidersManager* colMan;
    ResourceManager* resMan;
    std::shared_ptr<cmp::Line> line;
    
    std::shared_ptr<cmp::Shader> turretShader;
    std::shared_ptr<cmp::Shader> lineShader;

    
    //for public READ

    bool isPlacing = false;

    //for public SET

    bool isLookingAtMirror = false;


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

        selectedTurretType = TurretType::Laser;
        multiTool->SetActiveIcon(selectedTurretType);
    }

    void Update(float dt)
    {
        bool needUpdate = false; 

        if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr1) && unlocked[0])
        {
            selectedTurretType = (TurretType)0;
            needUpdate = true;
        }
        else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr2) && unlocked[1])
        {
            selectedTurretType = (TurretType)1;
            needUpdate = true;
        }
        else if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr3) && unlocked[2])
        {
            selectedTurretType = (TurretType)2;
            needUpdate = true;
        }


        short scroll = Input()->Mouse()->GetScrollOffset();
        if (scroll != 0)
        {
            short newType = ((selectedTurretType - scroll) % 3 + 3) % 3;
            if (unlocked[newType])
            {
                selectedTurretType = (TurretType)newType;
                needUpdate = true;
            }
            else
            {
                newType = ((newType - scroll) % 3 + 3) % 3;
                if (unlocked[newType])
                {
                    selectedTurretType = (TurretType)newType;
                    needUpdate = true;
                }
            }
        }

        if (needUpdate)
        {
            multiTool->SetActiveIcon(selectedTurretType);
        }


        int lineIndexToPlace = 2;
        bool hasPlacedTurret = false;

        turretPrefabs[0]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);
        turretPrefabs[1]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);
        turretPrefabs[2]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);

        if (isPlacing)
        {
            if (Input()->Mouse()->OnPressed(MouseButton::Left_MB) && gameManager->GetCurrentEnergy() >= turretCosts[selectedTurretType]) 
            {
                hasPlacedTurret = true;
            }
            else if (Input()->Mouse()->OnPressed(MouseButton::Right_MB))
            {
                isPlacing = false;
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

            if (selectedTurretType != TurretType::None)
            {
                glm::vec3 adjust = {0.0f, 0.0f, 0.0f};
                turretPrefabs[selectedTurretType]->GetComponent<cmp::Transform>()->SetPosition(line->Get(lineIndexToPlace) + transform->GetPosition() + adjust);
                turretPrefabs[selectedTurretType]->GetComponent<cmp::Transform>()->SetRotation(0.0f, -camera->GetYaw(), 0.0f);
            }   
        }
        else
        {
            line->Set(2, {0.0f, 999.9f, 0.0f}); //'delete' turret xd
            if (!isLookingAtMirror && Input()->Mouse()->OnPressed(MouseButton::Left_MB) && selectedTurretType != TurretType::None && unlocked[selectedTurretType])
            {
                isPlacing = true;
            }
        }

        if (hasPlacedTurret)
        {
            if (auto scriptHolder = turretPrefabs[selectedTurretType]->GetComponent<cmp::Scriptable>())
            {
                scriptHolder->EnableAll();
                
                std::shared_ptr<ColliderComponent> col = nullptr;
                col = turretPrefabs[selectedTurretType]->GetComponent<cmp::SphereCol>();
                if (!col) col = turretPrefabs[selectedTurretType]->GetComponent<cmp::BoxCol>();
                if(col)
                {
                    col->AddToCollidersManager(colMan);
                }
            }

            
            auto model = turretPrefabs[selectedTurretType]->GetComponent<cmp::Model>();
            if(!model)
                if (auto gfxNode = turretPrefabs[selectedTurretType]->GetNode()->FindNode("gfx"))
                    model = gfxNode->GetGameObject()->GetComponent<cmp::Model>();
            
            if (model)
                model->SetTintColor(1.0, 1.0, 1.0);


            CreateTurret(selectedTurretType);
            gameManager->DescreaseEnergy(turretCosts[selectedTurretType]);
            // selectedTurretType = TurretType::None;
            // multiTool->SetActiveIcon(selectedTurretType);
            isPlacing = false;
        }
    }

    void CreateTurret(TurretType type)
    {
        printf("Creating new turret of type %i\n", type);
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
    }

    void CreateLanternTurret()
    {
        TurretType type = TurretType::Lantern;
        turretPrefabs[type] = std::make_shared<GameObject>();
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::Transform>());
        turretPrefabs[type]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);


        auto gfxGO = std::make_shared<GameObject>();
        gfxGO->AddComponent(std::make_shared<cmp::Transform>());
        gfxGO->AddComponent(std::make_shared<cmp::Name>("gfx"));

        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Wieze/Latarnia.obj"),
            resMan->GetMaterial("Resources/models/Wieze/Latarnia.mtl")
        );
        gfxGO->AddComponent(mc);
        mc->SetTintColor(turretGhostColor);
        
        gfxGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        gfxGO->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Wieze/Latarnia.obj")
        );
        gfxGO->AddComponent(turretShader);


        // gfxGO->AddComponent(std::make_shared<cmp::Shade>());
        // std::shared_ptr<cmp::Shade> shadeCmp = gfxGO->GetComponent<cmp::Shade>();
        // shadeCmp->Create(1);


        turretsHolder->AddChild(turretPrefabs[type])->AddChild(gfxGO);
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
        mc->SetTintColor(turretGhostColor);
        turretPrefabs[type]->AddComponent(turretShader);
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::Transform>());
        turretPrefabs[type]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::FrustumCulling>());
        turretPrefabs[type]->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Wieze/Strzelajaca.obj"));
        
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        std::shared_ptr<cmp::BoxCol> col = turretPrefabs[type]->GetComponent<cmp::BoxCol>();
        col->setLengths({2.5, 3.5, 2.5});
        col->SetOffset({0.0, 2.0, 0.0});

        turretPrefabs[type]->AddComponent(std::make_shared<cmp::Scriptable>());
        TurretShoot* script = new TurretShoot();
        script->isPut = true;
        script->SetEnabled(false);
        turretPrefabs[type]->GetComponent<cmp::Scriptable>()->Add(script);

        
        auto rangeGO = std::make_shared<GameObject>();
        rangeGO->AddComponent(std::make_shared<cmp::Transform>());

        rangeGO->AddComponent(std::make_shared<cmp::SphereCol>(true, true));
        std::shared_ptr<cmp::SphereCol> rangeCol = rangeGO->GetComponent<cmp::SphereCol>();
        rangeCol->SetRadius(shootingTurretRange);
        rangeCol->AddToCollidersManager(colMan);

        rangeGO->AddComponent(std::make_shared<cmp::Scriptable>());
        TurretRange* range = new TurretRange();
        script->turretRange = range;
        rangeGO->GetComponent<cmp::Scriptable>()->Add(range);
        

        turretsHolder->AddChild(turretPrefabs[type])->AddChild(rangeGO);
    }

    void CreateLaserTurret()
    {
        TurretType type = TurretType::Laser;
        turretPrefabs[type] = std::make_shared<GameObject>();
        turretPrefabs[type]->AddComponent(std::make_shared<cmp::Transform>());
        turretPrefabs[type]->GetComponent<cmp::Transform>()->SetPosition(0.0f, 999.9f, 0.0f);


        auto scriptHolder = std::make_shared<cmp::Scriptable>();
        turretPrefabs[type]->AddComponent(scriptHolder);

        auto turretScript = new TurretLaser();
        turretScript->colMan = colMan;
        turretScript->SetEnabled(false);
        
        auto line = std::make_shared<cmp::Line>();
        line->Create();
        line->thickness = 2.0f;
        line->color1 = { 1.0f, 1.0f, 0.0f };
        line->color2 = { 1.0f, 0.7f, 0.0f };

        turretPrefabs[type]->AddComponent(line);
        turretPrefabs[type]->AddComponent(lineShader);
        turretScript->line = line.get();


        scriptHolder->Add(turretScript);


        turretPrefabs[type]->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        std::shared_ptr<cmp::BoxCol> col = turretPrefabs[type]->GetComponent<cmp::BoxCol>();
        col->setLengths({2.0, 2.5, 2.0});
        col->SetOffset({0.0, 1.75, 0.0});
        


        auto gfxGO = std::make_shared<GameObject>();
        gfxGO->AddComponent(std::make_shared<cmp::Transform>());
        gfxGO->AddComponent(std::make_shared<cmp::Name>("gfx"));

        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Wieze/Laser.obj"),
            resMan->GetMaterial("Resources/models/Wieze/Laser.mtl")
        );
        mc->SetTintColor(turretGhostColor);
        gfxGO->AddComponent(mc);
        
        gfxGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        gfxGO->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Wieze/Laser.obj")
        );
        gfxGO->AddComponent(turretShader);


        turretsHolder->AddChild(turretPrefabs[type])->AddChild(gfxGO);
    }

    void PickUpTurret(TurretType type, std::shared_ptr<GameObject> turretGO)
    {    
        if (type == TurretType::None) return;
        gameManager->IncreaseEnergy(turretCosts[type]);

        turretGO->GetComponent<cmp::Transform>()->SetPosition(0.0, -10000.0, 0.0);

        //removing doesnt work lol

        // printf("PickUpTurret()\n");

        // std::shared_ptr<ColliderComponent> col = turretGO->GetComponent<cmp::SphereCol>();
        // if (col == nullptr)
        // {
        //     col = gameObject->GetComponent<cmp::BoxCol>();
        // }
        // if (col != nullptr)
        // {
        //     printf(" has collider\n");
            
        //     printf("a\n");
        //     colMan->RemoveStaticTrigger(col);
        //     printf("b\n");
        //     colMan->RemoveDynamicTrigger(col);
        //     printf("c\n");
        //     colMan->RemoveStaticColllider(col);
        //     printf("d\n");
        //     colMan->RemoveDynamicCollider(col);
        // }
        
        // turretsHolder->RemoveNode(turretGO->GetNode());
    }

};