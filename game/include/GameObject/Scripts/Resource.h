#pragma once

#include "GameApplication.h"
#include "Components.h"
#include "GameObject.h"
#include "GameManager.h"

#include <iostream>


class Resource : public Script
{
public:
	int energy = 100;
	
	void Start()
	{

	}

	void Update(float dt)
	{

	}

	void TriggerEnter(std::shared_ptr<ColliderComponent> collider)
	{
		std::shared_ptr<GameObject> player = collider->GetOwner();
		std::shared_ptr<cmp::Name> playerName = player->GetComponent<cmp::Name>();
		if (playerName != nullptr && playerName->Get() == "CAMERA")
		{
			player->GetComponent<cmp::Scriptable>()->Get<GameManager>()->IncreaseEnergy(energy);
			gameObject->GetComponent<cmp::Transform>()->SetPosition(0, -100, 0);
		}
	}
};