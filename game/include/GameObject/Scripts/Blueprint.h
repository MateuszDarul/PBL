#pragma once

#include "GameApplication.h"
#include "Components.h"
#include "GameObject.h"
#include "SoundPlayer.h"

#include "PlayerPlaceTurret.h"
#include "MultiToolController.h"


class Blueprint : public Script
{
public:
	PlayerPlaceTurret::TurretType type;

	SoundPlayer* pickupSFX;
	
	void Start()
	{
		if (!pickupSFX) pickupSFX = new SoundPlayer("Resources/sounds/blueprint.wav");
		pickupSFX->SetVolume(0.5f);
	}

	~Blueprint()
	{
		delete pickupSFX;
	}

	void Update(float dt)
	{

	}

	void PickUp()
	{
		auto player = gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject();
		if (player)
		{
			pickupSFX->Play();
			player->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->unlocked[type] = true;
            gameObject->GetNode()->GetRoot()->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<MultiToolController>()->Unlock(type);			
            gameObject->GetComponent<cmp::Transform>()->SetPosition(0, -100, 0);
		}
	}
};