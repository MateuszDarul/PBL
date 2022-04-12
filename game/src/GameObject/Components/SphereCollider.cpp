#include "SphereCollider.h"
#include "Components.h"
#include "GameObject.h"


SphereCollider::SphereCollider(GameObject* gameObject)
	:ColliderComponent(11, gameObject, nullptr, false, false)
{
	radius = 1.0f;
}

SphereCollider::SphereCollider(GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic)
	:ColliderComponent(11, gameObject, collidersManager, isTrigger, isStatic)
{
	radius = 1.0f;
}
SphereCollider::~SphereCollider()
{

}

bool SphereCollider::CheckCollision(ColliderComponent* collider)
{
	TransformComponent* thisTransform = this->GetGameObject()->GetComponent<TransformComponent>();
	TransformComponent* otherTransform = collider->GetGameObject()->GetComponent<TransformComponent>();
	glm::mat4 thisModelMat = thisTransform->GetModelMatrix();
	glm::vec3 thisPos = glm::vec3(thisModelMat[3][0], thisModelMat[3][1], thisModelMat[3][2]) + this->offset;
	glm::mat4 otherModelMat = otherTransform->GetModelMatrix();
	glm::vec3 otherPos = glm::vec3(otherModelMat[3][0], otherModelMat[3][1], otherModelMat[3][2]) + this->offset;
	if (collider->GetClassUUID() == 11)
	{
		SphereCollider* other = (SphereCollider*) collider;
		float thisRadius = this->radius;
		float otherRadius = other->radius;
		glm::vec3 thisVec = glm::normalize(otherPos - thisPos) * thisRadius;
		glm::vec3 otherVec = glm::normalize(thisPos - otherPos) * otherRadius;
		float distance = glm::distance(thisPos, otherPos);
		float radiusDiff = distance - thisRadius - otherRadius;
		if (radiusDiff < 0)
		{
			bool thisMoves = !this->isStatic && !this->isTrigger;
			bool otherMoves = !other->isStatic && !other->isTrigger;
			if (thisMoves && otherMoves)
			{
				thisVec *= 0.5 * -radiusDiff / radius;
				otherVec *= 0.5 * -radiusDiff / radius;
				thisTransform->SetPosition(thisTransform->GetPosition() + otherVec);
				otherTransform->SetPosition(otherTransform->GetPosition() + thisVec);
			}
			else if(thisMoves)
			{
				otherVec *= -radiusDiff / radius;
				thisTransform->SetPosition(thisTransform->GetPosition() + otherVec);
			}
			else if (otherMoves)
			{
				thisVec *= -radiusDiff / radius;
				otherTransform->SetPosition(otherTransform->GetPosition() + otherVec);
			}
			return true;
		}
	}
	return false;
}

void SphereCollider::SetRadius(float radius)
{
	if (radius < 0)
	{
		this->radius = 0;
	}
	else
	{
		this->radius = radius;
	}
}

float SphereCollider::GetRadius()
{
	return radius;
}

