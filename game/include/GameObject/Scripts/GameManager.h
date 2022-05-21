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
		energy = 100;
		energyText = gameObject->GetNode()->GetParent()->FindNode("EnergyText")->GetGameObject()->GetComponent<cmp::Text>();
	}

	void Update(float dt)
	{
		energyText->SetText("Energy: " + std::to_string(energy));
	}

	void IncreaseEnergy(int energy)
	{
		this->energy += energy;
		std::cout << this->energy<<std::endl;
	}
	void DescreaseEnergy(int energy)
	{
		this->energy -= energy;
		std::cout << this->energy << std::endl;
	}

	int GetCurrentEnergy()
	{
		return energy;
	}
};