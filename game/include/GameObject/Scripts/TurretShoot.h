#pragma once

#include <vector>
#include "Components.h"
#include "GameObject.h"
#include "SceneNode.h"
#include "ColliderComponent.h"
#include "Scene.h"
#include "TurretRange.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TurretShoot : public Script
{
public:
	TurretRange* turretRange;
private:
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
		if (isPut && turretRange)
		{
			if (turretRange->enemies.size() > 0)
			{
				float angle = Angle(gameObject->GetComponent<cmp::Transform>()->GetPosition(), turretRange->enemies[0]->gameObject->GetComponent<cmp::Transform>()->GetPosition());
				gameObject->GetComponent<cmp::Transform>()->SetRotation(0, glm::degrees(angle), 0);
			}

			if (shootTimer < shootCd)
			{
				shootTimer += dt;
			}
			else
			{
				if (turretRange->enemies.size() > 0)
				{
					turretRange->enemies[0]->DecreaseHealth(damage);
					shootTimer = 0.0f;
				}
			}
		}	
	}

	
};