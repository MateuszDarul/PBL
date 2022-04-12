#ifndef __BOX_COLLIDER_H__
#define __BOX_COLLIDER_H__
#include "ColliderComponent.h"

#include <glm/glm.hpp>

class BoxCollider : public ColliderComponent
{
public:
	BoxCollider(GameObject*);
	BoxCollider(GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic);
	~BoxCollider();
	bool virtual CheckCollision(ColliderComponent* collider);
};
#endif