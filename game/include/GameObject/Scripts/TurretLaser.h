#pragma once

#include "GameApplication.h"
#include "Components.h"

#include "DoorActivator.h"
#include "PlacedTurret.h"
#include "TurretLight.h"

#include <glm/gtx/rotate_vector.hpp>


class TurretLaser : public Script
{
public:
    
    //adjust these

    int ignoreLayerMask = ~(CollisionLayer::Player);
    int maxBounces = 15;
    float maxDistance = 150.0f;

    const char* mirrorTag = "Mirror";

    glm::vec3 originOffset = { 0.0f, 2.3f, 0.0f };


    //set these in 'inspector'

    cmp::Line* line;
    CollidersManager* colMan;
    PlacedTurret* placedTurretScript;

 
private:

    glm::vec3 dir = { 1.0f, 0.0f, 0.0f };
    glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
   
    cmp::Transform* transform;


public:

    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>().get();
    }

    void Update(float dt)
    {	
        if (line && placedTurretScript->IsWorking())
        {
            origin = transform->GetPosition() + originOffset;
            line->Set(0, origin);
            dir = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(transform->GetRotation().y));


            float d = maxDistance;
            int hits = 0;
            int totalLinePoints = 2;

            RayHitInfo hit;
            while (hits < maxBounces && d > 0.0f)
            {
                if (colMan->Raycast(origin, dir, hit, d, false, ignoreLayerMask))
                {
                    hits += 1;
                    d -= hit.distance;

                    if (line->Count()-1 < hits)
                    {
                        //line->AddPoint(hit.point - transform->GetPosition());
                        line->AddPoint(hit.point);
                    }
                    else
                    {
                        //line->Set(hits, hit.point - transform->GetPosition());
                        line->Set(hits, hit.point);
                    }

                    auto nameCmp = hit.gameObject->GetComponent<cmp::Name>();
                    TurretLight* lightScript = nullptr;
                    auto lightScriptable = hit.gameObject->GetNode()->GetParent()->GetGameObject()->GetComponent<cmp::Scriptable>();
                    if (lightScriptable)
                    {
                        lightScript = lightScriptable->Get<TurretLight>();
                    }
                    if (!nameCmp && !lightScript) break;

                    if (nameCmp)
                    {
                        if (nameCmp->Get().compare(mirrorTag) == 0)
                        {
                            dir = glm::reflect(dir, hit.normal);
                            origin = hit.point;
                            totalLinePoints += 1;
                            continue;
                        }
                    }

                    if (lightScript)
                    {
                        if (lightScript != placedTurretScript->GetLight())
                        {
                            lightScript->light->GetComponent<cmp::PointLight>()->TurnOn();
                        }
                        break;
                    }

                    auto scriptHolder = hit.gameObject->GetComponent<cmp::Scriptable>();
                    if (scriptHolder)
                    {
                        if (auto doorActivator = scriptHolder->Get<DoorActivator>())
                        {
                            doorActivator->Activate();
                        }
                    }
                    
                    break;
                }
                else
                {
                    if (line->Count() < totalLinePoints)
                    {
                        line->AddPoint(origin + dir * d);
                    }  
                    else
                    {
                        line->Set(totalLinePoints-1, origin + dir * d);
                    }

                    break;
                }
            }

            if (line->Count() > totalLinePoints)
            {
                line->RemoveLast(line->Count() - totalLinePoints);
            }
        }
    }
};