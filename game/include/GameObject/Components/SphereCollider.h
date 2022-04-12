#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__
#include "ColliderComponent.h"
#include <glm/glm.hpp>

class SphereCollider : public ColliderComponent
{
public:
	float radius;
	SphereCollider(GameObject*);
	SphereCollider(GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic);
	~SphereCollider();
	bool virtual CheckCollision(ColliderComponent* collider);
};
#endif