#include "BoxCollider.h"
#include "Components.h"
#include "GameObject.h"

BoxCollider::BoxCollider(GameObject* gameObject)
	:ColliderComponent(12, gameObject, nullptr, false, false)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	isStatic = false;
	isTrigger = false;
}

BoxCollider::BoxCollider(GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic) 
	:ColliderComponent(12, gameObject, collidersManager, isTrigger, isStatic)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	isStatic = false;
	isTrigger = false;
}

BoxCollider::~BoxCollider()
{

}

bool BoxCollider::CheckCollision(ColliderComponent* collider)
{
	return false;
}

