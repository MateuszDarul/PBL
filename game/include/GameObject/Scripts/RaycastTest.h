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

    float distance = 1.0f;
    float hitDistance;
    glm::vec3 dir = { 1.0f, 0.0f, 0.0f };
    glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
    int currentIndex;

    cmp::Transform* transform;

    void Start()
    {
        transform = gameObject->GetComponent<cmp::Transform>().get();

        dir = normalize(dir);

        if (line)
        {
            origin = line->Get(0) + transform->GetPosition();  
            currentIndex = 1;
            line->AddPoint(0, 0, 0);
         //   hitDistance = GetNextDistance();
        }
    }

    void LineStuff()
    {
        auto& p1 = line->Get(1);

        glm::vec3 normal = { -1.0f, 0.0f, 0.0f }; 
        normal = glm::rotate( 
            normal, 
            glm::radians(collisionTarget->GetRotation().y),
            {0.0f, 1.0f, 0.0f}
        );
    
        glm::vec3 otherPos = collisionTarget->GetPosition();
        glm::vec3 c = normal * (float)collisionTarget->GetOwner()->GetComponent<cmp::BoxCol>()->getLengths().x;
        otherPos += c;
        
        glm::vec3 w = otherPos - line->Get(0);

        p1 = otherPos;

        auto& p2 = line->Get(2);

        p2 = glm::reflect(p1 - origin, normal);

        p2 -= gameObject->GetComponent<cmp::Transform>()->GetPosition();
        p1 -= gameObject->GetComponent<cmp::Transform>()->GetPosition();

        //float k = glm::dot(w, normal) / glm::dot(dir, normal);
    }

    void Raycast()
    {
        glm::vec3 normal(collisionTarget->GetModelMatrix() * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
        normal -= collisionTarget->GetPosition();
        normal = glm::normalize(normal);

        auto col = collisionTarget->GetOwner()->GetComponent<cmp::BoxCol>()->getLengths();

        glm::vec4 v0_4 = collisionTarget->GetModelMatrix() * glm::vec4(-0.5f * col.x,  0.5f * col.y,  0.5f * col.z, 1.0);
        glm::vec4 v1_4 = collisionTarget->GetModelMatrix() * glm::vec4(-0.5f * col.x, -0.5f * col.y,  0.5f * col.z, 1.0);
        glm::vec4 v2_4 = collisionTarget->GetModelMatrix() * glm::vec4(-0.5f * col.x, -0.5f * col.y, -0.5f * col.z, 1.0);
        glm::vec4 v4_4 = collisionTarget->GetModelMatrix() * glm::vec4(-0.5f * col.x,  0.5f * col.y, -0.5f * col.z, 1.0);

        glm::vec3 v0(v0_4);
        glm::vec3 v1(v1_4);
        glm::vec3 v2(v2_4);
        glm::vec3 v4(v4_4);

        glm::vec2 bary1, bary2;
        float maxDistance = 14.0f;
        float d1 = maxDistance;
        float d2 = maxDistance;
        float d  = maxDistance;
        if (glm::intersectRayTriangle(origin, dir, v0, v1, v2, bary1, d1)
         |  glm::intersectRayTriangle(origin, dir, v2, v4, v0, bary2, d2))
        {
            d = std::min(d1, d2);
            line->Get(2) = (maxDistance - d) * glm::reflect(dir, normal);
        }
        else
        {
            line->Get(2) = line->Get(0);;
        }


        printf("Triangle:\n %f %f %f\n %f %f %f\n %f %f %f\n %f %f %f\n",
            v0.x, v0.y, v0.z,
            v1.x, v1.y, v1.z,
            v2.x, v2.y, v2.z,
            v4.x, v4.y, v4.z
        );
        printf("Bary1: %f, %f \t%f\n", bary1.x, bary1.y, d1);
        printf("Bary2: %f, %f \t%f\n", bary2.x, bary2.y, d2);
        //printf("dist %f\n", d);
        
        line->Get(1).x = d;
    }

    float animation = 0.0f;

    void Update(float dt)
    {
        if (line)
        {
            origin = line->Get(0) + transform->GetPosition();  
            if (collisionTarget)
            {
                Raycast();
            }
        }

        glm::vec3 pos = transform->GetPosition();
        pos.y = 2.5f * sin(animation * 0.1f) + 2.0f;
        transform->SetPosition(pos);

        animation += dt;
    }
};