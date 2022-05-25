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

	void PickUp()
	{
		auto player = gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject();
		if (player)
		{
			player->GetComponent<cmp::Scriptable>()->Get<GameManager>()->IncreaseEnergy(energy);
			gameObject->GetComponent<cmp::Transform>()->SetPosition(0, -100, 0);
		}
	}
};