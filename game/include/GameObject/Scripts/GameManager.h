#pragma once

#include "GameApplication.h"
#include "Components.h"
#include "GameObject.h"

#include <iostream>


class GameManager : public Script
{
private:
	int energy;
	std::shared_ptr<cmp::Text> energyText;
public:

	void Start()
	{
		energy = 0;
		if (auto energyGuiNode = gameObject->GetNode()->GetRoot()->FindNode("EnergyText"))
		{
			energyText = energyGuiNode->GetGameObject()->GetComponent<cmp::Text>();
		}
		UpdateResourcesText();
	}

	void Update(float dt)
	{
	}

	void UpdateResourcesText()
	{
		if (energyText)
		{
			energyText->SetText(std::to_string(energy));
			
			float w = energyText->GetWidth();
			energyText->GetOwner()->GetComponent<cmp::Transform>()->SetPosition(-w*0.5f, -0.035f, 0.0f);
		}
	}

	void IncreaseEnergy(int energy)
	{
		this->energy += energy;
		std::cout << "Increasing energy: " << this->energy<<std::endl;
		UpdateResourcesText();
	}
	void DescreaseEnergy(int energy)
	{
		this->energy -= energy;
		std::cout << "Decreasing energy: " << this->energy << std::endl;
		UpdateResourcesText();
	}

	int GetCurrentEnergy()
	{
		return energy;
	}
};