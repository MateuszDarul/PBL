#pragma once

#include "Components.h"

#include "Health.h"


class TurretRange : public Script
{
public:
    std::vector<Health*> enemies;

    void Start()
    {
        
    }

    void Update(float dt)
    {
        
    }

    void TriggerEnter(std::shared_ptr<ColliderComponent> collider)
	{
		if (collider->layer == ENEMY)
		{
			Health* health = collider->GetOwner()->GetComponent<cmp::Scriptable>()->Get<Health>();
			if (health != nullptr)
			{
				enemies.push_back(health);
			}
		}
	}
	void TriggerExit(std::shared_ptr<ColliderComponent> collider)
	{
		if (collider->layer == ENEMY)
		{
			Health* health = collider->GetOwner()->GetComponent<cmp::Scriptable>()->Get<Health>();
			for (int i = 0; i < enemies.size(); i++)
			{
				if (enemies[i] == health)
				{
					enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
					break;
				}
			}
		}
	}
};