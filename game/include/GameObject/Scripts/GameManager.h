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
	bool immortal = false;
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
			std::string text = std::to_string(energy);
			if (energy < 100) text = "0" + text;
			if (energy < 10 ) text = "0" + text;
			energyText->SetText(text);
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