
#include "ColliderComponent.h"
#include "CollidersManager.h"
#include "GameObject.h"

#include <iostream>

int ColliderComponent::GetLowestValueIndex(float array[6])
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

void ColliderComponent::GetSeparationVectors(float array[6], glm::vec3& boxMoveVec, glm::vec3& sphereMoveVec)
{
	int index = GetLowestValueIndex(array);
	switch (index)
	{
	case 0:
		sphereMoveVec.x = -array[index];
		boxMoveVec.x = array[index];
		break;
	case 1:
		sphereMoveVec.x = array[index];
		boxMoveVec.x = -array[index];
		break;
	case 2:
		sphereMoveVec.z = -array[index];
		boxMoveVec.z = array[index];
		break;
	case 3:
		sphereMoveVec.z = array[index];
		boxMoveVec.z = -array[index];
		break;
	case 4:
		sphereMoveVec.y = -array[index];
		boxMoveVec.y = array[index];
		break;
	case 5:
		sphereMoveVec.y = array[index];
		boxMoveVec.y = -array[index];
		break;
	}
}

ColliderComponent::ColliderComponent(uint32_t UUID, bool isTrigger, bool isStatic, int layer)
	:Component(UUID), isTrigger(isTrigger), isStatic(isStatic), layer(layer)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	mass = 1.0f;
	isOptimized = false;
}

ColliderComponent::~ColliderComponent()
{

}

void ColliderComponent::AddToCollidersManager(CollidersManager* collidersManager)
{
	if (collidersManager)
	{
		if (isTrigger)
		{
			if (isStatic)
			{
				collidersManager->AddStaticTrigger(std::dynamic_pointer_cast<ColliderComponent>(shared_from_this()));
			}
			else
			{
				collidersManager->AddDynamicTrigger(std::dynamic_pointer_cast<ColliderComponent>(shared_from_this()));
			}
		}
		else
		{
			if (isStatic)
			{
				collidersManager->AddStaticCollider(std::dynamic_pointer_cast<ColliderComponent>(shared_from_this()));
			}
			else
			{
				collidersManager->AddDynamicCollider(std::dynamic_pointer_cast<ColliderComponent>(shared_from_this()));
			}
		}
	}
}

glm::vec3 ColliderComponent::GetOffset()
{
	return offset;
}

void ColliderComponent::SetOffset(glm::vec3 offset)
{
	this->offset = offset;
}

float ColliderComponent::Clamp(float spherePos, float boxMinSidePos, float boxMaxSidePos)
{
	if (spherePos < boxMinSidePos)
	{
		return boxMinSidePos;
	}
	else if (spherePos > boxMaxSidePos)
	{
		return boxMaxSidePos;
	}
	return spherePos;
}

void ColliderComponent::SetMass(float mass)
{
	if (mass > 0)
	{
		this->mass = mass;
	}
}

float ColliderComponent::GetMass()
{
	return mass;
}

