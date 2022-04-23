#include "CollidersManager.h"
#include <iostream>

void CollidersManager::RemoveFromVector(std::vector<ColliderComponent*> vector, ColliderComponent* col)
{
	int size = vector.size();
	for (int i = 0; i < size; i++)
	{
		if (vector[i] == col)
		{
			vector.erase(vector.begin() + i);
			break;
		}
	}
}

CollidersManager::CollidersManager()
{
}

CollidersManager::~CollidersManager()
{
	dynamicColliders.clear();
	staticColliders.clear();
	dynamicTriggers.clear();
	staticTriggers.clear();
}

void CollidersManager::AddStaticCollider(ColliderComponent* collider)
{
	staticColliders.push_back(collider);
}

void CollidersManager::AddDynamicCollider(ColliderComponent* collider)
{
	dynamicColliders.push_back(collider);
}

void CollidersManager::AddStaticTrigger(ColliderComponent* trigger)
{
	staticTriggers.push_back(trigger);
}

void CollidersManager::AddDynamicTrigger(ColliderComponent* trigger)
{
	dynamicTriggers.push_back(trigger);
}

void CollidersManager::RemoveDynamicColllider(ColliderComponent* collider)
{
	RemoveFromVector(dynamicColliders, collider);
}

void CollidersManager::RemoveStaticColllider(ColliderComponent* collider)
{
	RemoveFromVector(staticColliders, collider);
}

void CollidersManager::RemoveDynamicTrigger(ColliderComponent* trigger)
{
	RemoveFromVector(dynamicTriggers, trigger);
}

void CollidersManager::RemoveStaticTrigger(ColliderComponent* trigger)
{
	RemoveFromVector(staticTriggers, trigger);
}

void CollidersManager::CheckCollisions()
{
	//std::cout << dynamicColliders.size();
	for (int i = 0; i<dynamicColliders.size(); i++)
	{
		for (int j = i + 1; j < dynamicColliders.size(); j++)
		{
			dynamicColliders[i]->CheckCollision(dynamicColliders[j]);
		}
		for (int j = 0; j < staticColliders.size(); j++)
		{
			dynamicColliders[i]->CheckCollision(staticColliders[j]);
		}
		for (int j = 0; j < dynamicTriggers.size(); j++)
		{
			dynamicColliders[i]->CheckCollision(dynamicTriggers[j]);
		}
		for (int j = 0; j < staticTriggers.size(); j++)
		{
			dynamicColliders[i]->CheckCollision(staticTriggers[j]);
		}
	}
}

void CollidersManager::CheckTriggers()
{
	for (int i = 0; i < dynamicTriggers.size(); i++)
	{
		for (int j = i + 1; j < dynamicTriggers.size(); j++)
		{
			dynamicColliders[i]->CheckCollision(dynamicTriggers[j]);
		}
		for (int j = 0; j < staticColliders.size(); j++)
		{
			dynamicColliders[i]->CheckCollision(staticColliders[j]);
		}
		for (int j = 0; j < staticTriggers.size(); j++)
		{
			dynamicColliders[i]->CheckCollision(staticTriggers[j]);
		}
	}
}
