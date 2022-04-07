#include "BoxCollider.h"
BoxCollider::BoxCollider(bool isTrigger, bool isStatic) :ColliderComponent(12, isTrigger, isStatic)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	isStatic = false;
	isTrigger = false;
}

BoxCollider::~BoxCollider()
{

}

