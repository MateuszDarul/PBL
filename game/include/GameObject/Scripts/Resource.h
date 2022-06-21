#pragma once

#include "GameApplication.h"
#include "Components.h"
#include "GameObject.h"
#include "GameManager.h"
#include "SoundPlayer.h"

#include <iostream>


class Resource : public Script
{
public:
	int energy = 100;
	
	SoundPlayer* pickupSFX;

	void Start()
	{
		if (!pickupSFX) pickupSFX = new SoundPlayer("Resources/sounds/resource.wav");
		pickupSFX->SetVolume(0.2f);
	}

	~Resource()
	{
		delete pickupSFX;
	}

	void Update(float dt)
	{ }

	void PickUp()
	{
		auto player = gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject();
		if (player)
		{
			pickupSFX->Play();
			player->GetComponent<cmp::Scriptable>()->Get<GameManager>()->IncreaseEnergy(energy);
			gameObject->GetComponent<cmp::Transform>()->SetPosition(0, -100, 0);
		}
	}
};