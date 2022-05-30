#include "CollidersManager.h"
#include "GameApplication.h"
#include "GameObject.h"
#include "SceneNode.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include <iostream>

void CollidersManager::RemoveFromVector(std::vector<std::weak_ptr<ColliderComponent>> *vector, std::shared_ptr<ColliderComponent> col)
{
	int size = vector->size();
	for (int i = 0; i < size; i++)
	{
		std::shared_ptr<ColliderComponent> collider = vector->at(i).lock();
		if (collider == col)
		{
			vector->erase(vector->begin() + i, vector->begin() + i + 1);
			break;
		}
	}
}

void CollidersManager::ExecuteOnCollidingScripts(std::vector<std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>>> *currentCollisions,
	std::shared_ptr<ColliderComponent> firstCollider, std::shared_ptr<ColliderComponent> secondCollider, bool areColliding, CollidingType type)
{
	std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>> pair1 = std::make_pair(firstCollider, secondCollider);
	std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>> pair2 = std::make_pair(secondCollider, firstCollider);
	std::shared_ptr<cmp::Scriptable> script = nullptr;
	bool found = false;
	for (int i = 0; i < recentCollisions.size(); i++)
	{
		if (recentCollisions[i] == pair1 || recentCollisions[i] == pair2)
		{
			found = true;
			break;
		}
	}
	if (found)
	{
		if (areColliding)
		{
			currentCollisions->push_back(pair1);
			script = firstCollider->GetOwner()->GetComponent<cmp::Scriptable>();
			if (script != nullptr)
			{
				if (type == collision)
				{
					script->OnCollisionStay(secondCollider);
				}
				else if (type == trigger)
				{
					script->OnTriggerStay(secondCollider);
				}
			}
			script = secondCollider->GetOwner()->GetComponent<cmp::Scriptable>();
			if (script != nullptr)
			{
				if (type == collision)
				{
					script->OnCollisionStay(firstCollider);
				}
				else if (type == trigger)
				{
					script->OnTriggerStay(firstCollider);
				}
			}
		}
		else
		{
			script = firstCollider->GetOwner()->GetComponent<cmp::Scriptable>();
			if (script != nullptr)
			{
				if (type == collision)
				{
					script->OnCollisionExit(secondCollider);
				}
				else if (type == trigger)
				{
					script->OnTriggerExit(secondCollider);
				}
			}
			script = secondCollider->GetOwner()->GetComponent<cmp::Scriptable>();
			if (script != nullptr)
			{
				if (type == collision)
				{
					script->OnCollisionExit(firstCollider);
				}
				else if (type == trigger)
				{
					script->OnTriggerExit(firstCollider);
				}
			}
		}
	}
	else
	{
		if (areColliding)
		{
			currentCollisions->push_back(pair1); 
			script = firstCollider->GetOwner()->GetComponent<cmp::Scriptable>();
			if (script != nullptr)
			{
				if (type == collision)
				{
					script->OnCollisionEnter(secondCollider);
				}
				else if (type == trigger)
				{
					script->OnTriggerEnter(secondCollider);
				}
			}
			script = secondCollider->GetOwner()->GetComponent<cmp::Scriptable>();
			if (script != nullptr)
			{
				if (type == collision)
				{
					script->OnCollisionEnter(firstCollider);
				}
				else if (type == trigger)
				{
					script->OnTriggerEnter(firstCollider);
				}
			}
		}
	}
}

void CollidersManager::OnRemoveExecuteScript(std::vector<std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>>>* currentCollisions,
	std::shared_ptr<ColliderComponent> removedCollider)
{
	std::shared_ptr<cmp::Scriptable> script = nullptr;
	std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>> pair;
	for (int i = 0; i < currentCollisions->size(); i++)
	{
		pair = currentCollisions->at(i);
		if (pair.first == removedCollider) 
		{
			script = pair.second->GetOwner()->GetComponent<cmp::Scriptable>();
			if (script != nullptr)
			{
				if (pair.first->isTrigger || pair.second->isTrigger)
				{
					script->OnTriggerExit(pair.first);
				}
				else
				{
					script->OnCollisionExit(pair.first);
				}
			}
		}
		else if (pair.second == removedCollider)
		{
			script = pair.first->GetOwner()->GetComponent<cmp::Scriptable>();
			if (script != nullptr)
			{
				if (pair.first->isTrigger || pair.second->isTrigger)
				{
					script->OnTriggerExit(pair.second);
				}
				else
				{
					script->OnCollisionExit(pair.second);
				}
			}
		}
	}
}

CollidersManager::CollidersManager(std::shared_ptr<GameObject> player): player(player)
{
	distanceFromPlayer = 0.0f;
}

bool CollidersManager::CheckOptimalization(std::shared_ptr<ColliderComponent> collider, Frustum frustum, glm::vec3 playerPos)
{
	bool optimize = collider->isOptimized;
	std::shared_ptr<FrustumCullingComponent> colliderFrustum = collider->GetOwner()->GetComponent<FrustumCullingComponent>();
	if (!optimize //wykona si� je�li nie jest optymalizowany
		|| (optimize //lub jest optymalizowany
			&& (glm::distance(collider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer //i odleg�o�� od gracza jest mniejsza ni� warto�� w managerze
				|| (colliderFrustum != nullptr && //albo ma component frustum 
					colliderFrustum->IsVisible(frustum))))) // i jest w obszarze kamery
	{
		return true;
	}
	return false;
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

void CollidersManager::RemoveDynamicCollider(std::shared_ptr<ColliderComponent> collider)
{
	OnRemoveExecuteScript(&recentCollisions, collider);
	RemoveFromVector(&dynamicColliders, collider);
}

void CollidersManager::RemoveStaticColllider(std::shared_ptr<ColliderComponent> collider)
{
	OnRemoveExecuteScript(&recentCollisions, collider);
	RemoveFromVector(&staticColliders, collider);
}

void CollidersManager::RemoveDynamicTrigger(std::shared_ptr<ColliderComponent> trigger)
{
	OnRemoveExecuteScript(&recentCollisions, trigger);
	RemoveFromVector(&dynamicTriggers, trigger);
}

void CollidersManager::RemoveStaticTrigger(std::shared_ptr<ColliderComponent> trigger)
{
	OnRemoveExecuteScript(&recentCollisions, trigger);
	RemoveFromVector(&staticTriggers, trigger);
}

void CollidersManager::CheckCollisions(std::vector<std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>>> *currentCollisions)
{
	std::shared_ptr<CameraComponent> playerCam = player->GetComponent<CameraComponent>();
	Frustum frustum = SceneNode::cameraFrustum;
	glm::vec3 playerPos = playerCam->GetPosition();
	for (unsigned int i = 0; i<dynamicColliders.size(); i++)
	{
		std::shared_ptr<ColliderComponent> firstCollider = dynamicColliders[i].lock();
		if(CheckOptimalization(firstCollider, frustum, playerPos))
		{
			for (unsigned int j = i + 1; j < dynamicColliders.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = dynamicColliders[j].lock();
				if(CheckOptimalization(secondCollider, frustum, playerPos))
				{
					bool areColliding = firstCollider->CheckCollision(secondCollider);
					ExecuteOnCollidingScripts(currentCollisions, firstCollider, secondCollider, areColliding, collision);
				}
			}
			for (unsigned int j = 0; j < staticColliders.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = staticColliders[j].lock();
				if (CheckOptimalization(secondCollider, frustum, playerPos))
				{
					bool areColliding = firstCollider->CheckCollision(secondCollider);
					ExecuteOnCollidingScripts(currentCollisions, firstCollider, secondCollider, areColliding, collision);
				}
			}
			for (unsigned int j = 0; j < dynamicTriggers.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = dynamicTriggers[j].lock();
				if (CheckOptimalization(secondCollider, frustum, playerPos))
				{
					bool areColliding = firstCollider->CheckCollision(secondCollider);
					ExecuteOnCollidingScripts(currentCollisions, firstCollider, secondCollider, areColliding, trigger);
				}
			}
			for (unsigned int j = 0; j < staticTriggers.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = staticTriggers[j].lock();
				if (CheckOptimalization(secondCollider, frustum, playerPos))
				{

					bool areColliding = firstCollider->CheckCollision(secondCollider);
					ExecuteOnCollidingScripts(currentCollisions, firstCollider, secondCollider, areColliding, trigger);
				}
			}
		}
	}
}

void CollidersManager::CheckTriggers(std::vector<std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>>> *currentCollisions)
{
	std::shared_ptr<CameraComponent> playerCam= player->GetComponent<CameraComponent>();
	Frustum frustum = SceneNode::cameraFrustum;
	glm::vec3 playerPos = playerCam->GetPosition();
	for (unsigned int i = 0; i < dynamicTriggers.size(); i++)
	{
		std::shared_ptr<ColliderComponent> firstCollider = dynamicTriggers[i].lock();
		if (CheckOptimalization(firstCollider, frustum, playerPos))
		{
			for (unsigned int j = i + 1; j < dynamicTriggers.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = dynamicTriggers[j].lock();
				if (CheckOptimalization(secondCollider, frustum, playerPos))
				{
					bool areColliding = firstCollider->CheckCollision(secondCollider);
					ExecuteOnCollidingScripts(currentCollisions, firstCollider, secondCollider, areColliding, trigger);
				}
			}
			for (unsigned int j = 0; j < staticColliders.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = staticColliders[j].lock();
				if (CheckOptimalization(secondCollider, frustum, playerPos))
				{
					bool areColliding = firstCollider->CheckCollision(secondCollider);
					ExecuteOnCollidingScripts(currentCollisions, firstCollider, secondCollider, areColliding, trigger);
				}
			}
			for (unsigned int j = 0; j < staticTriggers.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = staticTriggers[j].lock();
				if (CheckOptimalization(secondCollider, frustum, playerPos))
				{
					bool areColliding = firstCollider->CheckCollision(secondCollider);
					ExecuteOnCollidingScripts(currentCollisions, firstCollider, secondCollider, areColliding, trigger);
				}
			}
		}
	}
}

void CollidersManager::CheckEverything()
{
	std::vector<std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>>> currentCollisions;
	CheckCollisions(&currentCollisions);
	CheckTriggers(&currentCollisions);
	recentCollisions = currentCollisions;
}

bool CollidersManager::Raycast(const glm::vec3& origin, const glm::vec3 dir, RayHitInfo& hitInfo, float maxDistance, bool shouldHitTriggers, int layerMask)
{
	//FIXME: inefficient checks 

	bool hit = false;
	RayHitInfo closestHit;
	closestHit.distance = maxDistance;

	// std::shared_ptr<CameraComponent> playerCam = player->GetComponent<CameraComponent>();
	// Frustum frustum = SceneNode::cameraFrustum;
	// glm::vec3 playerPos = playerCam->GetPosition();

	for (unsigned int i = 0; i<dynamicColliders.size(); i++)
	{
		std::shared_ptr<ColliderComponent> collider = dynamicColliders[i].lock();

		if ((collider->layer & layerMask) && collider->RayCollision(origin, dir, hitInfo, maxDistance) && (hitInfo.distance < closestHit.distance))
		{
			hit = true;
			closestHit = hitInfo;
		}
	}

	for (unsigned int i = 0; i<staticColliders.size(); i++)
	{
		std::shared_ptr<ColliderComponent> collider = staticColliders[i].lock();

		if ((collider->layer & layerMask) && collider->RayCollision(origin, dir, hitInfo, maxDistance) && (hitInfo.distance < closestHit.distance))
		{
			hit = true;
			closestHit = hitInfo;
		}
	}

	if (shouldHitTriggers)
	{
		for (unsigned int i = 0; i<dynamicTriggers.size(); i++)
		{
			std::shared_ptr<ColliderComponent> collider = dynamicTriggers[i].lock();

			if ((collider->layer & layerMask) && collider->RayCollision(origin, dir, hitInfo, maxDistance) && (hitInfo.distance < closestHit.distance))
			{
				hit = true;
				closestHit = hitInfo;
			}
		}

		for (unsigned int i = 0; i<staticTriggers.size(); i++)
		{
			std::shared_ptr<ColliderComponent> collider = staticTriggers[i].lock();

			if ((collider->layer & layerMask) && collider->RayCollision(origin, dir, hitInfo, maxDistance) && (hitInfo.distance < closestHit.distance))
			{
				hit = true;
				closestHit = hitInfo;
			}
		}
	}

	hitInfo = closestHit;
	return hit;
}

void CollidersManager::SetDistanceFromPlayer(float distance)
{
	distanceFromPlayer = distance;
}

float CollidersManager::GetDistanceFromPlayer()
{
	return distanceFromPlayer;
}
