#include "CollidersManager.h"

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

void CollidersManager::AddStaticColllider(ColliderComponent* collider)
{
	staticColliders.push_back(collider);
}

void CollidersManager::AddDynamicColllider(ColliderComponent* collider)
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
}

void CollidersManager::CheckTriggers()
{
}
