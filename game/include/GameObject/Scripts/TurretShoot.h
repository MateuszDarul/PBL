#pragma once

#include <vector>
#include "Components.h"
#include "GameObject.h"
#include "SceneNode.h"
#include "ColliderComponent.h"
#include "Scene.h"
#include "Health.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TurretShoot : public Script
{
private:
	std::vector<Health*> enemies;
	float shootTimer = 1.0f;
	float Angle(glm::vec3 origin, glm::vec3 other)
	{
		float toReturn = glm::acos(glm::dot(glm::vec3(1, 0, 0), glm::normalize(other - origin)));
		if (origin.z - other.z <0)
		{
			toReturn = -toReturn;
		}
		return toReturn;
	}
public:
	float damage = 10.0f;
	float shootCd = 1.0f;
	bool isPut = false;

	void Start()
	{

	}

	void Update(float dt)
	{
		if (isPut)
		{
			if (shootTimer < shootCd)
			{
				shootTimer += dt;
			}
			else
			{
				if (enemies.size() > 0)
				{
					float angle = Angle(gameObject->GetComponent<cmp::Transform>()->GetPosition(), enemies[0]->gameObject->GetComponent<cmp::Transform>()->GetPosition());
					gameObject->GetComponent<cmp::Transform>()->SetRotation(0, glm::degrees(angle), 0);
					enemies[0]->DecreaseHealth(damage);
					shootTimer = 0.0f;
				}
			}
		}	
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