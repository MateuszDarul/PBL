#ifndef __COLLIDER_COMPONENT_H__
#define __COLLIDER_COMPONENT_H__
#include "Component.h"
#include "CollidersManager.h"
#include <glm/glm.hpp>

class ColliderComponent : public Component
{
protected:
	glm::vec3 offset;

public:
	bool isStatic;
	bool isTrigger;
	ColliderComponent(uint32_t UUID, bool isTrigger, bool isStatic);
	~ColliderComponent();
	glm::vec3 GetOffset();
	void SetOffset(glm::vec3 offset);
	bool virtual CheckCollision(ColliderComponent collider);
};
#endif