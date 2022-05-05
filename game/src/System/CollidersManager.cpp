#include "CollidersManager.h"
#include "GameApplication.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
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

CollidersManager::CollidersManager(std::shared_ptr<GameObject> player): player(player)
{
	distanceFromPlayer = 0.0f;
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
	std::shared_ptr<CameraComponent> playerCam = player->GetComponent<CameraComponent>();
	Frustum frustum = playerCam->GetFrustum((float)GameApplication::GetWindowSize().x / GameApplication::GetWindowSize().y,
		GameApplication::GetFov(),
		GameApplication::GetProjectionRange().x,
		GameApplication::GetProjectionRange().y);
	glm::vec3 playerPos = playerCam->GetPosition();
	for (unsigned int i = 0; i<dynamicColliders.size(); i++)
	{
		std::shared_ptr<ColliderComponent> firstCollider = dynamicColliders[i].lock();
		bool firstOptimize = firstCollider->isOptimized;
		std::shared_ptr<FrustumCullingComponent> firstFrustum = firstCollider->GetOwner()->GetComponent<FrustumCullingComponent>();
		if (!firstOptimize || (firstOptimize && (glm::distance(firstCollider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer
			|| (firstFrustum != nullptr &&
				firstFrustum->IsVisible(frustum))))) 
		{
			bool secondOptimize = false;
			std::shared_ptr<FrustumCullingComponent> secondFrustum;
			for (unsigned int j = i + 1; j < dynamicColliders.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = dynamicColliders[j].lock();
				secondFrustum = secondCollider->GetOwner()->GetComponent<FrustumCullingComponent>();
				secondOptimize = secondCollider->isOptimized;
				if (!secondOptimize || (secondOptimize && (glm::distance(secondCollider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer
					|| (secondFrustum != nullptr &&
						secondFrustum->IsVisible(frustum)))))
				{
					firstCollider->CheckCollision(secondCollider);
				}
			}
			for (unsigned int j = 0; j < staticColliders.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = staticColliders[j].lock();
				secondOptimize = secondCollider->isOptimized;
				if (!secondOptimize || (secondOptimize && (glm::distance(secondCollider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer
					|| (secondFrustum != nullptr &&
						secondFrustum->IsVisible(frustum)))))
				{
					firstCollider->CheckCollision(secondCollider);
				}
			}
			for (unsigned int j = 0; j < dynamicTriggers.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = dynamicTriggers[j].lock();
				secondOptimize = secondCollider->isOptimized;
				if (!secondOptimize || (secondOptimize && (glm::distance(secondCollider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer
					|| (secondFrustum != nullptr &&
						secondFrustum->IsVisible(frustum)))))
				{
					firstCollider->CheckCollision(secondCollider);
				}
			}
			for (unsigned int j = 0; j < staticTriggers.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = staticTriggers[j].lock();
				secondOptimize = secondCollider->isOptimized;
				if (!secondOptimize || (secondOptimize && (glm::distance(secondCollider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer
					|| (secondFrustum != nullptr &&
						secondFrustum->IsVisible(frustum)))))
				{
					firstCollider->CheckCollision(secondCollider);
				}
			}
		}
	}
}

void CollidersManager::CheckTriggers()
{
	std::shared_ptr<CameraComponent> playerCam= player->GetComponent<CameraComponent>();
	Frustum frustum = playerCam->GetFrustum((float)GameApplication::GetWindowSize().x / GameApplication::GetWindowSize().y,
		GameApplication::GetFov(),
		GameApplication::GetProjectionRange().x,
		GameApplication::GetProjectionRange().y);
	glm::vec3 playerPos = playerCam->GetPosition();
	for (unsigned int i = 0; i < dynamicTriggers.size(); i++)
	{
		std::shared_ptr<ColliderComponent> firstCollider = dynamicColliders[i].lock();
		bool firstOptimize = firstCollider->isOptimized;
		std::shared_ptr<FrustumCullingComponent> firstFrustum = firstCollider->GetOwner()->GetComponent<FrustumCullingComponent>();
		if (!firstOptimize || (firstOptimize && (glm::distance(firstCollider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer
			|| (firstFrustum != nullptr && 
				firstFrustum->IsVisible(frustum)))))
		{
			bool secondOptimize = false;
			std::shared_ptr<FrustumCullingComponent> secondFrustum;
			for (unsigned int j = i + 1; j < dynamicTriggers.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = dynamicTriggers[j].lock();
				secondFrustum = secondCollider->GetOwner()->GetComponent<FrustumCullingComponent>();
				secondOptimize = secondCollider->isOptimized;
				if (!secondOptimize || (secondOptimize && (glm::distance(secondCollider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer
					|| (secondFrustum != nullptr &&
						secondFrustum->IsVisible(frustum)))))
				{
					firstCollider->CheckCollision(secondCollider);
				}
			}
			for (unsigned int j = 0; j < staticColliders.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = staticColliders[j].lock();
				secondOptimize = secondCollider->isOptimized;
				if (!secondOptimize || (secondOptimize && (glm::distance(secondCollider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer
					|| (secondFrustum != nullptr &&
					secondFrustum->IsVisible(frustum)))))
				{
					firstCollider->CheckCollision(secondCollider);
				}
			}
			for (unsigned int j = 0; j < staticTriggers.size(); j++)
			{
				std::shared_ptr<ColliderComponent> secondCollider = staticTriggers[j].lock();
				secondOptimize = secondCollider->isOptimized;
				if (!secondOptimize || (secondOptimize && (glm::distance(secondCollider->GetOwner()->GetComponent<TransformComponent>()->GetPosition(), playerPos) <= distanceFromPlayer
					|| (secondFrustum != nullptr &&
						secondFrustum->IsVisible(frustum)))))
				{
					firstCollider->CheckCollision(secondCollider);
				}
			}
		}
	}
}

void CollidersManager::SetDistanceFromPlayer(float distance)
{
	distanceFromPlayer = distance;
}

float CollidersManager::GetDistanceFromPlayer()
{
	return distanceFromPlayer;
}
