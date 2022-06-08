#pragma once

#include "GameApplication.h"
#include "Components.h"
#include "GameManager.h"
#include "TurretLight.h"

#include <iostream>


class PlacedTurret : public Script
{
private:
	TurretLight* light = nullptr;
	bool isWorking;
public:
	void Start()
	{
		isWorking = false;
	}

	void Update(float dt)
	{
		if (light != nullptr)
		{
			//std::cout << light->isLight<<std::endl;
		}
		if (light != nullptr && light->isLight)
		{
			isWorking = true;
		}
		else
		{
			isWorking = false;
		}
	}

	bool IsWorking()
	{
		return isWorking;
	}

	void TriggerEnter(std::shared_ptr<ColliderComponent> collider)
	{
		if (collider->layer == CollisionLayer::Light)
		{
			light = collider->GetOwner()->GetComponent<cmp::Scriptable>()->Get<TurretLight>();
			std::cout << "sdafdss";
		}
	}

	void TriggerExit(std::shared_ptr<ColliderComponent> collider)
	{
		if (collider->layer == CollisionLayer::Light)
		{
			light = nullptr;
		}
	}

	TurretLight* GetLight()
	{
		return light;
	}
};