#pragma once

#include "GameApplication.h"
#include "Components.h"

#include "DoorActivator.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

#include <iostream>


class RaycastTest : public Script
{
public:
    
    //adjust these

    int ignoreLayerMask = ~(CollisionLayer::Player);
    int maxBounces = 15;
    float maxDistance = 50.0f;

    const char* mirrorTag = "Mirror";


    //set these in 'inspector'

    cmp::Line* line;
    CollidersManager* colMan;


 
private:

    glm::vec3 dir = { 1.0f, 0.0f, 0.0f };
    glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
   
    cmp::Transform* transform;


public:

    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>().get();

        // dir = normalize(dir);

        // // if (line)
        // // {
        // //     origin = line->Get(0) + transform->GetPosition();  
        // //     line->AddPoint(0, 0, 0);
        // // }
    }

    void Update(float dt)
    {	
        if (line)
        {
            //origin = line->Get(0) + transform->GetPosition();
            origin = line->Get(0);
            dir = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(transform->GetRotation().y));
            // dir  = glm::vec3(transform->GetModelMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            // dir -= transform->GetPosition();
            // dir  = glm::normalize(dir);


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
                    if (!nameCmp) break;

                    if (nameCmp->Get().compare(mirrorTag) == 0)
                    {
                        dir = glm::reflect(dir, hit.normal);
                        origin = hit.point;
                        totalLinePoints += 1;
                        continue;
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