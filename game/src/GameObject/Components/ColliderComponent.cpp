#include "ColliderComponent.h"
ColliderComponent::ColliderComponent(uint32_t UUID, bool isTrigger, bool isStatic):Component(UUID), isTrigger(isTrigger), isStatic(isStatic)
{
	if(isTrigger)
	{
		if (isStatic)
		{
			
		}
	}
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
}

ColliderComponent::~ColliderComponent()
{

}

glm::vec3 ColliderComponent::GetOffset()
{
	return offset;
}

void ColliderComponent::SetOffset(glm::vec3 offset)
{
	this->offset = offset;
}

