#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__
#include "ColliderComponent.h"
#include <glm/glm.hpp>

class SphereCollider : public ColliderComponent
{
public:
	float radius;
	SphereCollider(bool isTrigger, bool isStatic);
	~SphereCollider();
};
#endif