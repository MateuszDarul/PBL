#include "SphereCollider.h"
SphereCollider::SphereCollider(bool isTrigger, bool isStatic) :ColliderComponent(11, isTrigger, isStatic)
{
	radius = 1.0f;
}
SphereCollider::~SphereCollider()
{

}

