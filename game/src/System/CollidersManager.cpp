#include "CollidersManager.h"
#include <iostream>

void CollidersManager::RemoveFromVector(std::vector<std::weak_ptr<ColliderComponent>> vector, std::shared_ptr<ColliderComponent> col)
{
	int size = vector.size();
	for (int i = 0; i < size; i++)
	{
		std::shared_ptr<ColliderComponent> collider = vector[i].lock();
		if (collider == col)
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

void CollidersManager::AddStaticCollider(std::shared_ptr<ColliderComponent> collider)
{
	staticColliders.push_back(collider);
}

void CollidersManager::AddDynamicCollider(std::shared_ptr<ColliderComponent> collider)
{
	dynamicColliders.push_back(collider);
}

void CollidersManager::AddStaticTrigger(std::shared_ptr<ColliderComponent> trigger)
{
	staticTriggers.push_back(trigger);
}

void CollidersManager::AddDynamicTrigger(std::shared_ptr<ColliderComponent> trigger)
{
	dynamicTriggers.push_back(trigger);
}

void CollidersManager::RemoveDynamicColllider(std::shared_ptr<ColliderComponent> collider)
{
	RemoveFromVector(dynamicColliders, collider);
}

void CollidersManager::RemoveStaticColllider(std::shared_ptr<ColliderComponent> collider)
{
	RemoveFromVector(staticColliders, collider);
}

void CollidersManager::RemoveDynamicTrigger(std::shared_ptr<ColliderComponent> trigger)
{
	RemoveFromVector(dynamicTriggers, trigger);
}

void CollidersManager::RemoveStaticTrigger(std::shared_ptr<ColliderComponent> trigger)
{
	RemoveFromVector(staticTriggers, trigger);
}

void CollidersManager::CheckCollisions()
{
	//std::cout << dynamicColliders.size();
	for (int i = 0; i<dynamicColliders.size(); i++)
	{
		std::shared_ptr<ColliderComponent> firstCollider = dynamicColliders[i].lock();
		for (int j = i + 1; j < dynamicColliders.size(); j++)
		{
			std::shared_ptr<ColliderComponent> secondCollider = dynamicColliders[j].lock();
			firstCollider->CheckCollision(secondCollider);
		}
		for (int j = 0; j < staticColliders.size(); j++)
		{
			std::shared_ptr<ColliderComponent> secondCollider = staticColliders[j].lock();
			firstCollider->CheckCollision(secondCollider);
		}
		for (int j = 0; j < dynamicTriggers.size(); j++)
		{
			std::shared_ptr<ColliderComponent> secondCollider = dynamicTriggers[j].lock();
			firstCollider->CheckCollision(secondCollider);
		}
		for (int j = 0; j < staticTriggers.size(); j++)
		{
			std::shared_ptr<ColliderComponent> secondCollider = staticTriggers[j].lock();
			firstCollider->CheckCollision(secondCollider);
		}
	}
}

void CollidersManager::CheckTriggers()
{
	for (int i = 0; i < dynamicTriggers.size(); i++)
	{
		std::shared_ptr<ColliderComponent> firstCollider = dynamicColliders[i].lock();
		for (int j = i + 1; j < dynamicTriggers.size(); j++)
		{
			std::shared_ptr<ColliderComponent> secondCollider = dynamicTriggers[j].lock();
			firstCollider->CheckCollision(secondCollider);
		}
		for (int j = 0; j < staticColliders.size(); j++)
		{
			std::shared_ptr<ColliderComponent> secondCollider = staticColliders[j].lock();
			firstCollider->CheckCollision(secondCollider);
		}
		for (int j = 0; j < staticTriggers.size(); j++)
		{
			std::shared_ptr<ColliderComponent> secondCollider = staticTriggers[j].lock();
			firstCollider->CheckCollision(secondCollider);
		}
	}
}
