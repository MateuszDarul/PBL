#pragma once

#include "GameApplication.h"
#include "Components.h"


#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

#include <iostream>


class RaycastTest : public Script
{
public:
    cmp::Line* line;
    cmp::Transform* collisionTarget;

    CollidersManager* colMan;
 
    
    glm::vec3 dir = { 1.0f, 0.0f, 0.0f };
    glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
   
    cmp::Transform* transform;


    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>().get();

        dir = normalize(dir);

        if (line)
        {
            origin = line->Get(0) + transform->GetPosition();  
            line->AddPoint(0, 0, 0);
        }
    }

    void Update(float dt)
    {
        if (collisionTarget)
        {
            if(Input()->Keyboard()->IsPressed(KeyboardKey::Right))
            {
                collisionTarget->Rotate(0, -7.0f * dt, 0);
            }
            if(Input()->Keyboard()->IsPressed(KeyboardKey::Left))
            {
                collisionTarget->Rotate(0,  7.0f * dt, 0);
            }
        }

        if (line)
        {
            float maxDistance = 25.0f;
            int maxBounces = 10;

            origin = line->Get(0) + transform->GetPosition();
            float d = maxDistance;
            int hits = 0;
            dir = { 1.0f, 0.0f, 0.0f };

            RayHitInfo hit;
            while (hits < maxBounces && d > 0.0f && colMan->Raycast(origin, dir, hit, d))
            {
                hits += 1;
                d -= hit.distance;

                if (line->Count()-1 < hits)
                {
                    line->AddPoint(hit.point - transform->GetPosition());
                }
                else
                {
                    line->Get(hits) = hit.point - transform->GetPosition();
                }

                // if (hit.gameObject.layer == mirror)
                // {

                    printf("  old origin: %f, %f, %f\n", origin.x, origin.y, origin.z);
                    printf("old dir: %f, %f, %f\n", dir.x, dir.y, dir.z);
                    dir = glm::reflect(dir, hit.normal);
                    origin = hit.point;// + dir * 0.001f;
                // }
                // else
                // {
                //     if (hit.gameObject.layer == activable)
                //     {
                //         hit.gameObject.dostuff;
                //     }
                //     break;
                // }
                printf("New origin: %f, %f, %f\n", origin.x, origin.y, origin.z);
                printf("New dir: %f, %f, %f\n", dir.x, dir.y, dir.z);
                printf("New distance: %f\n", d);
                printf("Hit count: %i\n", hits);
            }

            if (line->Count() > hits + 2)
            {
                line->RemoveLast(line->Count() - hits - 2);
            }
            else if (line->Count() < hits + 2)
            {
                line->AddPoint(0,0,0);
            }
            

            line->Get(hits+1) = origin + dir * d - transform->GetPosition();
        }
    }
};