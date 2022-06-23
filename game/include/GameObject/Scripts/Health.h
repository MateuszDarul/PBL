#pragma once

#include "Components.h"
#include "GameObject.h"
#include "SceneNode.h"
#include "ColliderComponent.h"
#include "Scene.h"

class Health : public Script
{
private:
	float maxHealth = 100.0f;
	float health = 100.0f;
public:
	Scene* scene;

	void Start()
	{

	}

	void Update(float dt)
	{
	}

	void DecreaseHealth(float health)
	{
		this->health -= health;
		//std::cout << this->health<<std::endl;
		if (this->health <= 0)
		{
			// to nie powinno byc tutaj - co jesli nie chcemy usunac objektu nawet jak ma 0 hp?
			gameObject->GetNode()->GetParent()->RemoveNode(gameObject->GetNode());
		}
	}
	
	void AddHealth(float health)
	{
		this->health += health;
		if (this->health > maxHealth)
		{
			this->health = maxHealth;
		}
	}

	void SetMaxHealth(float health)
	{
		maxHealth = health;
		this->health = health;
	}
	
	float GetHealth()
	{
		return health;
	}
};