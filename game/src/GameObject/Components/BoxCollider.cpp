#include "BoxCollider.h"
#include "Components.h"
#include "GameObject.h"

int BoxCollider::GetLowestValueIndex(float array[6])
{
	float min = array[0];
	int index = 0;
	for (int i = 1; i < 6; i++)
	{
		if (array[i] < min)
		{
			min = array[i];
			index = i;
		}
	}
	return index;
}

BoxCollider::BoxCollider(GameObject* gameObject)
	:ColliderComponent(12, gameObject, nullptr, false, false)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	isStatic = false;
	isTrigger = false;
	xLength = 1.0f;
	yLength = 1.0f;
	zLength = 1.0f;
}

BoxCollider::BoxCollider(GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic) 
	:ColliderComponent(12, gameObject, collidersManager, isTrigger, isStatic)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	isStatic = false;
	isTrigger = false;
	xLength = 1.0f;
	yLength = 1.0f;
	zLength = 1.0f;
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
	if (collider->GetClassUUID() == 12)
	{
		BoxCollider* other = (BoxCollider*)collider;
		float thisMinX = thisPos.x - this->xLength * 0.5f;
		float thisMaxX = thisPos.x + this->xLength * 0.5f;
		float thisMinY = thisPos.y - this->yLength * 0.5f;
		float thisMaxY = thisPos.y + this->yLength * 0.5f;
		float thisMinZ = thisPos.z - this->zLength * 0.5f;
		float thisMaxZ = thisPos.z + this->zLength * 0.5f;
		float otherMinX = otherPos.x - other->xLength * 0.5f;
		float otherMaxX = otherPos.x + other->xLength * 0.5f;
		float otherMinY = otherPos.y - other->yLength * 0.5f;
		float otherMaxY = otherPos.y + other->yLength * 0.5f;
		float otherMinZ = otherPos.z - other->zLength * 0.5f;
		float otherMaxZ = otherPos.z + other->zLength * 0.5f;
		if (thisMinX <= otherMaxX && otherMinX <= thisMaxX &&
			thisMinZ <= otherMaxZ && otherMinZ <= thisMaxZ &&
			thisMinY <= otherMaxY && otherMinY <= thisMaxY)
		{
			bool thisMoves = !this->isStatic && !this->isTrigger;
			bool otherMoves = !other->isStatic && !other->isTrigger;
			if (thisMoves || otherMoves)
			{
				float array[] = { otherMaxX - thisMinX, thisMaxX - otherMinX, otherMaxZ - thisMinZ, thisMaxZ - otherMinZ, otherMaxY - thisMinY, thisMaxY - otherMinY };
				int index = GetLowestValueIndex(array);
				glm::vec3 thisMoveVec = { 0.0f,0.0f,0.0f };
				glm::vec3 otherMoveVec = { 0.0f,0.0f,0.0f };
				switch (index)
				{
				case 0:
					otherMoveVec.x = -array[index] * 0.5f;
					thisMoveVec.x = array[index] * 0.5f;
				case 1:
					otherMoveVec.x = array[index] * 0.5f;
					thisMoveVec.x = -array[index] * 0.5f;
				case 2:
					otherMoveVec.z = -array[index] * 0.5f;
					thisMoveVec.z = array[index] * 0.5f;
				case 3:
					otherMoveVec.z = array[index] * 0.5f;
					thisMoveVec.z = -array[index] * 0.5f;
				case 4:
					otherMoveVec.y = -array[index] * 0.5f;
					thisMoveVec.y = array[index] * 0.5f;
				case 5:
					otherMoveVec.y = array[index] * 0.5f;
					thisMoveVec.y = -array[index] * 0.5f;
				}
				if (thisMoves && otherMoves)
				{
					thisTransform->SetPosition(thisTransform->GetPosition() + thisMoveVec);
					otherTransform->SetPosition(otherTransform->GetPosition() + otherMoveVec);
				}
				else if (thisMoves)
				{
					thisTransform->SetPosition(thisTransform->GetPosition() + thisMoveVec - otherMoveVec);
				}
				else if (otherMoves)
				{
					otherTransform->SetPosition(otherTransform->GetPosition() + otherMoveVec - thisMoveVec);
				}
			}
			return true;
		}
	}
	return false;
}

