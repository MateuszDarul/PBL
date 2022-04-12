#ifndef __COLLIDERS_MANAGER_H__
#define __COLLIDERS_MANAGER_H__
#include "ColliderComponent.h"
#include <vector>

class CollidersManager 
{
private:
	std::vector<ColliderComponent*> staticColliders;
	std::vector<ColliderComponent*> staticTriggers;
	std::vector<ColliderComponent*> dynamicColliders;
	std::vector<ColliderComponent*> dynamicTriggers;
	void RemoveFromVector(std::vector<ColliderComponent*> vector, ColliderComponent* col);
public:
	CollidersManager();
	~CollidersManager();
	
	void AddStaticCollider(ColliderComponent* collider);
	void AddDynamicCollider(ColliderComponent* collider);
	void AddStaticTrigger(ColliderComponent* trigger);
	void AddDynamicTrigger(ColliderComponent* trigger);

	void RemoveDynamicColllider(ColliderComponent* collider);
	void RemoveStaticColllider(ColliderComponent* collider);
	void RemoveDynamicTrigger(ColliderComponent* trigger);
	void RemoveStaticTrigger(ColliderComponent* trigger);

	void CheckCollisions();
	void CheckTriggers();
};
#endif