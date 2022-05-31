#pragma once

#include "GameApplication.h"
#include "Components.h"
#include "GameObject.h"

#include "PlayerPlaceTurret.h"
#include "MultiToolController.h"


class Blueprint : public Script
{
public:
	int type = 0;
	
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
			player->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->unlocked[type] = true;
            gameObject->GetNode()->GetRoot()->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<MultiToolController>()->Unlock(type);			
            gameObject->GetComponent<cmp::Transform>()->SetPosition(0, -100, 0);
		}
	}
};