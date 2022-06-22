#pragma once

#include "GameApplication.h"
#include "Components.h"
#include "CollidersManager.h"
#include "SoundPlayer.h"

#include "Turret.h"
#include "DoorActivator.h"
#include "LightActivator.h"

#include <glm/gtx/rotate_vector.hpp>


class TurretLaser : public Turret
{
public:
    
    //adjust these

    int ignoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::Ignore);
    int maxBounces = 15;
    float maxDistance = 150.0f;

    float originOffsetUp = 2.38f;
    float originOffsetForward = 0.45f;


    //set these in 'inspector'

    cmp::Line* line;
    CollidersManager* colMan;

 
private:

    glm::vec3 dir = { 1.0f, 0.0f, 0.0f };
    glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
    glm::vec3 originOffset = { originOffsetForward, originOffsetUp, 0.0f };
   
    cmp::Transform* transform;

    SoundPlayer* mirrorSFX;
    int previousMirrorHitCount = 0;

public:

    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>().get();

        if (!mirrorSFX) mirrorSFX = new SoundPlayer("Resources/sounds/mirrorhit.wav");
    }

    void Update(float dt)
    {	
        if (line)
        {
            if (lightSourcesInRange < 1)
            {
                line->RemoveLast(line->Count() - 2);
                line->Set(0, glm::vec3(0,0,0));
                line->Set(1, glm::vec3(0,0,0));
                return;
            }

            dir = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(transform->GetRotation().y));
            originOffset = dir * originOffsetForward + glm::vec3(0.0f, 1.0f, 0.0f) * originOffsetUp;
            origin = transform->GetPosition() + originOffset;
            line->Set(0, origin);


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


                    if (hit.layer & CollisionLayer::Mirror)
                    {
                        if (previousMirrorHitCount < hits) mirrorSFX->Play();

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
                            line->Set(hits, hit.point + dir * 0.1f);
                        }
                        else if (auto lightActivator = scriptHolder->Get<LightActivator>())
                        {
                            if(lightSourcesInRange > 1 || !lightActivator->range->IsInRange(this))
                            {
                                lightActivator->Activate();
                            }
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

            previousMirrorHitCount = hits;
        }
    }
};