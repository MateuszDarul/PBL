#include "BoxCollider.h"
#include "Components.h"
#include "GameObject.h"

BoxCollider::BoxCollider(GameObject* gameObject)
	:ColliderComponent(12, gameObject, nullptr, false, false)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	isStatic = false;
	isTrigger = false;
	lengths = { 1.0f, 1.0f, 1.0f };
}

BoxCollider::BoxCollider(GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic) 
	:ColliderComponent(12, gameObject, collidersManager, isTrigger, isStatic)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	isStatic = false;
	isTrigger = false;
	lengths = { 1.0f, 1.0f, 1.0f };
}

BoxCollider::~BoxCollider()
{

}

bool BoxCollider::CheckCollision(ColliderComponent* collider)
{
	TransformComponent* thisTransform = this->GetGameObject()->GetComponent<TransformComponent>();
	TransformComponent* otherTransform = collider->GetGameObject()->GetComponent<TransformComponent>();
	glm::mat4 thisModelMat = thisTransform->GetModelMatrix();
	glm::vec3 thisPos = glm::vec3(thisModelMat[3][0], thisModelMat[3][1], thisModelMat[3][2]) + this->offset;
	glm::mat4 otherModelMat = otherTransform->GetModelMatrix();
	glm::vec3 otherPos = glm::vec3(otherModelMat[3][0], otherModelMat[3][1], otherModelMat[3][2]) + collider->GetOffset();

	glm::uvec3 thisLenghts = this->getLengths();
	float thisMinX = thisPos.x - thisLenghts.x * 0.5f;
	float thisMaxX = thisPos.x + thisLenghts.x * 0.5f;
	float thisMinY = thisPos.y - thisLenghts.y * 0.5f;
	float thisMaxY = thisPos.y + thisLenghts.y * 0.5f;
	float thisMinZ = thisPos.z - thisLenghts.z * 0.5f;
	float thisMaxZ = thisPos.z + thisLenghts.z * 0.5f;

	if (collider->GetClassUUID() == 11)
	{
		SphereCollider* other = (SphereCollider*)collider;
		float otherRadius = other->GetRadius();
		glm::vec3 closer = {
			Clamp(otherPos.x, thisMinX, thisMaxX),
			Clamp(otherPos.y, thisMinY, thisMaxY),
			Clamp(otherPos.z, thisMinZ, thisMaxZ)};
		float distance = glm::distance(closer, otherPos);
		if (distance < otherRadius)
		{
			bool thisMoves = !this->isStatic;
			bool otherMoves = !other->isStatic;
			if ((thisMoves || otherMoves) && !this->isTrigger && !other->isTrigger)
			{
				glm::vec3 thisMoveVec = { 0.0f,0.0f,0.0f };
				glm::vec3 otherMoveVec = { 0.0f,0.0f,0.0f };
				if (closer == otherPos)
				{
					float array[] = { glm::abs(otherPos.x - thisMinX + otherRadius), glm::abs(thisMaxX - otherPos.x + otherRadius),
						glm::abs(otherPos.z - thisMinZ + otherRadius), glm::abs(thisMaxZ - otherPos.z + otherRadius),
						glm::abs(otherPos.y - thisMinY + otherRadius), glm::abs(thisMaxY - otherPos.y + otherRadius) };
					GetSeparationVectors(array, thisMoveVec, otherMoveVec);
				}
				else
				{
					glm::vec3 moveVec = glm::normalize(otherPos - closer) * (otherRadius - distance);
					thisMoveVec = -moveVec;
					otherMoveVec = moveVec;
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
	else if (collider->GetClassUUID() == 12)
	{
		BoxCollider* other = (BoxCollider*)collider;
		glm::uvec3 otherLenghts = other->getLengths();
		float otherMinX = otherPos.x - otherLenghts.x * 0.5f;
		float otherMaxX = otherPos.x + otherLenghts.x * 0.5f;
		float otherMinY = otherPos.y - otherLenghts.y * 0.5f;
		float otherMaxY = otherPos.y + otherLenghts.y * 0.5f;
		float otherMinZ = otherPos.z - otherLenghts.z * 0.5f;
		float otherMaxZ = otherPos.z + otherLenghts.z * 0.5f;
		if (thisMinX <= otherMaxX && otherMinX <= thisMaxX &&
			thisMinZ <= otherMaxZ && otherMinZ <= thisMaxZ &&
			thisMinY <= otherMaxY && otherMinY <= thisMaxY)
		{
			bool thisMoves = !this->isStatic;
			bool otherMoves = !other->isStatic;
			if ((thisMoves || otherMoves) && !this->isTrigger && !other->isTrigger)
			{
				float array[] = { glm::abs(otherMaxX - thisMinX), glm::abs(thisMaxX - otherMinX), glm::abs(otherMaxZ - thisMinZ),
					glm::abs(thisMaxZ - otherMinZ), glm::abs(otherMaxY - thisMinY), glm::abs(thisMaxY - otherMinY) };
				glm::vec3 thisMoveVec = { 0.0f,0.0f,0.0f };
				glm::vec3 otherMoveVec = { 0.0f,0.0f,0.0f };
				GetSeparationVectors(array, thisMoveVec, otherMoveVec);
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

void BoxCollider::setLengths(glm::uvec3 lengths)
{
	this->lengths = lengths;
}

glm::uvec3 BoxCollider::getLengths()
{
	return lengths;
}

