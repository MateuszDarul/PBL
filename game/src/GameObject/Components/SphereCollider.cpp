#include "SphereCollider.h"
#include "Components.h"
#include "GameObject.h"

#include "CollidersManager.h"
#include <glm/gtx/intersect.hpp>

SphereCollider::SphereCollider()
	:ColliderComponent(11, false, false)
{
	radius = 1.0f;
}

SphereCollider::SphereCollider(bool isTrigger, bool isStatic)
	:ColliderComponent(11, isTrigger, isStatic)
{
	radius = 1.0f;
}
SphereCollider::~SphereCollider()
{

}

bool SphereCollider::CheckCollision(std::shared_ptr<ColliderComponent> collider)
{
	std::shared_ptr<TransformComponent> thisTransform = this->GetOwner()->GetComponent<TransformComponent>();
	std::shared_ptr<TransformComponent> otherTransform = collider->GetOwner()->GetComponent<TransformComponent>();
	glm::mat4 thisModelMat = thisTransform->GetModelMatrix();
	glm::vec3 thisPos = glm::vec3(thisModelMat[3][0], thisModelMat[3][1], thisModelMat[3][2]) + this->offset;
	glm::mat4 otherModelMat = otherTransform->GetModelMatrix();
	glm::vec3 otherPos = glm::vec3(otherModelMat[3][0], otherModelMat[3][1], otherModelMat[3][2]) + collider->GetOffset();
	if (collider->GetClassUUID() == 11)
	{
		std::shared_ptr<SphereCollider> other = std::dynamic_pointer_cast<SphereCollider>(collider);
		float thisRadius = this->radius;
		float otherRadius = other->radius;
		glm::vec3 thisVec = glm::normalize(otherPos - thisPos) * thisRadius;
		glm::vec3 otherVec = glm::normalize(thisPos - otherPos) * otherRadius;
		float distance = glm::distance(thisPos, otherPos);
		float radiusDiff = distance - thisRadius - otherRadius;
		if (radiusDiff < 0)
		{
			bool thisMoves = !this->isStatic;
			bool otherMoves = !other->isStatic;
			if (!this->isTrigger && !other->isTrigger)
			{
				if (thisMoves && otherMoves)
				{
					float thisMass = this->GetMass();
					float otherMass = other->GetMass();
					float massSum = thisMass + otherMass;
					thisVec *= otherMass / massSum * -radiusDiff / radius;
					otherVec *= thisMass / massSum * -radiusDiff / radius;
					thisTransform->SetPosition(thisTransform->GetPosition() + otherVec);
					otherTransform->SetPosition(otherTransform->GetPosition() + thisVec);
				}
				else if (thisMoves)
				{
					otherVec *= -radiusDiff / radius;
					thisTransform->SetPosition(thisTransform->GetPosition() + otherVec);
				}
				else if (otherMoves)
				{
					thisVec *= -radiusDiff / radius;
					otherTransform->SetPosition(otherTransform->GetPosition() + thisVec);
				}
			}
			return true;
		}
	}
	else if (collider->GetClassUUID() == 12)
	{
		std::shared_ptr<BoxCollider> other = std::dynamic_pointer_cast<BoxCollider>(collider);
		glm::uvec3 otherLenghts = other->getLengths();
		float otherMinX = otherPos.x - otherLenghts.x * 0.5f;
		float otherMaxX = otherPos.x + otherLenghts.x * 0.5f;
		float otherMinY = otherPos.y - otherLenghts.y * 0.5f;
		float otherMaxY = otherPos.y + otherLenghts.y * 0.5f;
		float otherMinZ = otherPos.z - otherLenghts.z * 0.5f;
		float otherMaxZ = otherPos.z + otherLenghts.z * 0.5f;

		float thisRadius = this->GetRadius();
		glm::vec3 closer = {
			Clamp(thisPos.x, otherMinX, otherMaxX),
			Clamp(thisPos.y, otherMinY, otherMaxY),
			Clamp(thisPos.z, otherMinZ, otherMaxZ)};
		float distance = glm::distance(closer, thisPos);
		if (distance < thisRadius)
		{
			bool thisMoves = !this->isStatic;
			bool otherMoves = !other->isStatic;
			if ((thisMoves || otherMoves) && !this->isTrigger && !other->isTrigger)
			{
				glm::vec3 thisMoveVec = { 0.0f,0.0f,0.0f };
				glm::vec3 otherMoveVec = { 0.0f,0.0f,0.0f };
				if (closer == thisPos)
				{
					float array[] = { glm::abs(thisPos.x - otherMinX + thisRadius), glm::abs(otherMaxX - thisPos.x + thisRadius),
						glm::abs(thisPos.z - otherMinZ + thisRadius), glm::abs(otherMaxZ - thisPos.z + thisRadius),
						glm::abs(thisPos.y - otherMinY + thisRadius), glm::abs(otherMaxY - thisPos.y + thisRadius) };
					GetSeparationVectors(array, otherMoveVec, thisMoveVec);
				}
				else
				{
					glm::vec3 moveVec = glm::normalize(thisPos - closer) * (thisRadius - distance);
					thisMoveVec = moveVec;
					otherMoveVec = -moveVec;
				}

				if (thisMoves && otherMoves)
				{
					float thisMass = this->GetMass();
					float otherMass = other->GetMass();
					float massSum = thisMass + otherMass;
					thisTransform->SetPosition(thisTransform->GetPosition() + thisMoveVec * otherMass / massSum);
					otherTransform->SetPosition(otherTransform->GetPosition() + otherMoveVec * thisMass / massSum);
				}
				else if (thisMoves)
				{
					thisTransform->SetPosition(thisTransform->GetPosition() + thisMoveVec);
				}
				else if (otherMoves)
				{
					otherTransform->SetPosition(otherTransform->GetPosition() + otherMoveVec);
				}
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

bool SphereCollider::RayCollision(const glm::vec3& origin, const glm::vec3 dir, RayHitInfo& hitInfo, float maxDistance)
{
	hitInfo.distance = maxDistance;

	auto center = GetOwner()->GetComponent<TransformComponent>()->GetPosition();

	if (glm::intersectRaySphere(origin, dir, center, radius*radius, hitInfo.distance))
	{
		if (hitInfo.distance < 0.0001f) return false;
		hitInfo.point = origin + dir * hitInfo.distance;
		hitInfo.normal = (hitInfo.point - center) / radius;
		hitInfo.gameObject = GetOwner().get();

		return true;
	}

	return false;
}