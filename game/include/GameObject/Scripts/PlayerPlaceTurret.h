#pragma once

#include "GameApplication.h"
#include "Components.h"


#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

#include <iostream>


class PlayerPlaceTurret : public Script
{
public:
    cmp::Line* line;
    

    CollidersManager* colMan;
    ResourceManager* resMan;
    std::shared_ptr<cmp::Shader> shader_l;
    Scene* scene;
    cmp::Transform* transform;
    cmp::Camera* camera;

    std::shared_ptr<GameObject> turretToPlace;

    bool isPlacing = false;
    float placingRange = 5.0f;


    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>().get();
        camera = gameObject->GetComponent<CameraComponent>().get();

        line->AddPoint(0, 0, 0);

        line->Set(0, {  0,  0,  0 });
        line->Set(1, {  1,  1, -2 });
        line->Set(2, {  1,  2, -3 });

        CreateTurret();
    }

    void Update(float dt)
    {
        if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr1))
        {
            isPlacing = !isPlacing;
        }
        //printf("%f %f %f\n", transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z);

        int indexToPlace = 2;

        if (isPlacing)
        {
            if (Input()->Mouse()->OnPressed(MouseButton::Left_MB))
            {
                CreateTurret();
            }

            RayHitInfo hit;
            if (colMan->Raycast(transform->GetPosition(), camera->GetForward(), hit, placingRange, false, ~CollisionLayer::Player))
            {
                line->Set(1, hit.point - transform->GetPosition());
            }
            else
            {
                line->Set(1, camera->GetForward() * placingRange); 
            }            

            if (colMan->Raycast(transform->GetPosition() + line->Get(1), {0.0f, -1.0f, 0.0f}, hit, 10.0f, false, ~CollisionLayer::Player))
            {
                line->Set(2, hit.point - transform->GetPosition());
            }
            else
            {
                line->Set(2, line->Get(1) + glm::vec3(0.0f, -1.0f, 0.0f));
                indexToPlace = 1;
            }  
            
        }
        else
        line->Set(2, {0.0f, 999.9f, 0.0f}); //xd

        if (turretToPlace)
        {
            glm::vec3 adjust = {0.0f, 0.0f, 0.0f};
            turretToPlace->GetComponent<cmp::Transform>()->SetPosition(line->Get(indexToPlace) + transform->GetPosition() + adjust);
            turretToPlace->GetComponent<cmp::Transform>()->SetRotation(0.0f, -camera->GetYaw(), 0.0f);
        }
    }

    void CreateTurret()
    {
        turretToPlace = std::make_shared<GameObject>();
    
        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj"),
            resMan->GetMaterial("Resources/models/wieze/w1/w1.mtl")
        );
        turretToPlace->AddComponent(mc);
        turretToPlace->AddComponent(shader_l);
        turretToPlace->AddComponent(std::make_shared<cmp::Transform>());
        turretToPlace->GetComponent<cmp::Transform>()->SetPosition(0,0.5,-5);
        turretToPlace->AddComponent(std::make_shared<cmp::FrustumCulling>());
        turretToPlace->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj")
        );
    
        scene->AddGameObject(turretToPlace);
    }
};