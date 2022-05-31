#pragma once

#include "Components.h"
#include "GameObject.h"


class PlayerGroundCheck : public Script
{
public:

    bool isGrounded = false;

    std::shared_ptr<cmp::Camera> player;

    void Start()
    {
        
    }

    void Update(float dt)
    {
        // if (isGrounded)
        //     printf("grounded\n");
        // else
        //     printf("not grounded\n");

        player->SetIsGrounded(isGrounded);

        isGrounded = false;
    }

    void TriggerStay(std::shared_ptr<ColliderComponent> other) override
    {
        if (!other->isTrigger && other->layer != CollisionLayer::Player)
        {
            isGrounded = true;
        }
    }

    // void TriggerEnter(std::shared_ptr<ColliderComponent> other) override
    // {
    //     if (other->GetOwner()->GetComponent<cmp::Camera>() && other->GetOwner()->GetComponent<cmp::Camera>() == player)
    //         isGrounded = true;

    //     auto otherNameCmp = other->GetOwner()->GetComponent<cmp::Name>();
    //     std::string otherName = (otherNameCmp) ? otherNameCmp->Get() : "Unnamed";
    //     if (otherName.compare("Resource 1") == 0)
    //     {
    //         printf("beka\n");
    //     }
    //     printf("trigger enter %s\n", otherName.c_str());
    // }

    // void TriggerExit(std::shared_ptr<ColliderComponent> other) override
    // {
    //     auto otherNameCmp = other->GetOwner()->GetComponent<cmp::Name>();
    //     const char* otherName = (otherNameCmp) ? otherNameCmp->Get().c_str() : "Unnamed";
    //     printf("exit trigger with %s\n", otherName);
    // }

    // void TriggerStay(std::shared_ptr<ColliderComponent> other) override
    // {
    //     auto otherNameCmp = other->GetOwner()->GetComponent<cmp::Name>();
    //     const char* otherName = (otherNameCmp) ? otherNameCmp->Get().c_str() : "Unnamed";
    //     printf("Triggering with %s\n", otherName);
    // }

    // void CollisionStay(std::shared_ptr<ColliderComponent> other) override
    // {
    //     auto otherNameCmp = other->GetOwner()->GetComponent<cmp::Name>();
    //     const char* otherName = (otherNameCmp) ? otherNameCmp->Get().c_str() : "Unnamed";
    //     printf("Colliding with %s\n", otherName);
    // }

    // void CollisionEnter(std::shared_ptr<ColliderComponent> other) override
    // {
    //     auto otherNameCmp = other->GetOwner()->GetComponent<cmp::Name>();
    //     const char* otherName = (otherNameCmp) ? otherNameCmp->Get().c_str() : "Unnamed";
    //     printf("entered collider with %s\n", otherName);
    // }

    // void CollisionExit(std::shared_ptr<ColliderComponent> other) override
    // {
    //     auto otherNameCmp = other->GetOwner()->GetComponent<cmp::Name>();
    //     const char* otherName = (otherNameCmp) ? otherNameCmp->Get().c_str() : "Unnamed";
    //     printf("exit collider with %s\n", otherName);
    // }
};