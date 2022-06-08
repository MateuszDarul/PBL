#pragma once

#include "GameApplication.h"
#include "Components.h"
#include "GameManager.h"
#include "Health.h"

#include <iostream>


class TurretLight : public Script
{
private:
	float range;
	std::vector<Health*> enemies;
	float shootTimer = 1.0f;

public:
	float shootCd = 1.0f;
	float damage = 2.0f;
	std::shared_ptr<GameObject> light;
	bool isLight;
	bool isAlarmed;
	void Start()
	{
		range = 10;
		isAlarmed = false;
		light->GetComponent<cmp::PointLight>()->SetPosition(gameObject->GetComponent<cmp::Transform>()->GetPosition() + glm::vec3(0, 3.5f, 0));
	}

	void Update(float dt)
	{
		if (shootTimer < shootCd)
		{
			shootTimer += dt;
		}
		else
		{
			if (enemies.size() > 0)
			{
				for (Health* enemy : enemies)
				{
					enemy->DecreaseHealth(damage);
				}
				shootTimer = 0.0f;
			}
		}
		isLight = light->GetComponent<cmp::PointLight>()->IsTurnedOn();
	}

	void SetRange(float range)
	{
		this->range = std::abs(range);
	}

	float GetRange()
	{
		return this->range;
	}

	void SetAlarmed(bool alarmed)
	{
		isAlarmed = alarmed;
	}

	bool IsAlarmed() 
	{
		return isAlarmed;
	}

	void TriggerStay(std::shared_ptr<ColliderComponent> collider)
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