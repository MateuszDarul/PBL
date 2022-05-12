#include "BoxCollider.h"
#include "Components.h"
#include "GameObject.h"

#include "CollidersManager.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

BoxCollider::BoxCollider()
	:ColliderComponent(12, false, false)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	isStatic = false;
	isTrigger = false;
	lengths = { 1.0f, 1.0f, 1.0f };
}

BoxCollider::BoxCollider(bool isTrigger, bool isStatic)
	:ColliderComponent(12, isTrigger, isStatic)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	lengths = { 1.0f, 1.0f, 1.0f };
}

BoxCollider::~BoxCollider()
{

}

bool BoxCollider::CheckCollision(std::shared_ptr<ColliderComponent> collider)
{
	std::shared_ptr<TransformComponent> thisTransform = this->GetOwner()->GetComponent<TransformComponent>();
	std::shared_ptr<TransformComponent> otherTransform = collider->GetOwner()->GetComponent<TransformComponent>();
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
		std::shared_ptr<SphereCollider> other = std::dynamic_pointer_cast<SphereCollider>(collider);
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
		std::shared_ptr<BoxCollider> other = std::dynamic_pointer_cast<BoxCollider>(collider);
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

bool BoxCollider::RayCollision(const glm::vec3& origin, const glm::vec3 dir, RayHitInfo& hitInfo, float maxDistance)
{
	auto transform = GetOwner()->GetComponent<cmp::Transform>();

	float d1, d2, d = maxDistance;
	glm::vec3 normal;
	glm::vec3 v0, v1, v2, v3;
	glm::vec2 bary1, bary2;

	// -X
	normal = glm::vec3(transform->GetModelMatrix() * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
    normal -= GetOwner()->GetComponent<cmp::Transform>()->GetPosition();
    normal = glm::normalize(normal);


	v0 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x,  0.5f * lengths.y,  0.5f * lengths.z, 1.0));
	v1 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, -0.5f * lengths.y,  0.5f * lengths.z, 1.0));
	v2 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, -0.5f * lengths.y, -0.5f * lengths.z, 1.0));
	v3 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x,  0.5f * lengths.y, -0.5f * lengths.z, 1.0));

	
	d1 = maxDistance;
	d2 = maxDistance;
	if (glm::dot(dir, normal) < 0 
    && (glm::intersectRayTriangle(origin, dir, v0, v1, v2, bary1, d1)
	||  glm::intersectRayTriangle(origin, dir, v2, v3, v0, bary2, d2)))	
	{
		d = std::min(d1, d2);
		if (d <= 0.0001f)
		{
			d = maxDistance;
		}
		else
		{
			hitInfo.point = origin + dir * d;
			hitInfo.normal = normal;
			hitInfo.distance = d;
			hitInfo.gameObject = GetOwner().get();

			return true;
		}
	}

	// +X
	normal = glm::vec3(transform->GetModelMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    normal -= GetOwner()->GetComponent<cmp::Transform>()->GetPosition();
    normal = glm::normalize(normal);


	v0 = glm::vec3(transform->GetModelMatrix() * glm::vec4(0.5f * lengths.x,  0.5f * lengths.y,  0.5f * lengths.z, 1.0));
	v1 = glm::vec3(transform->GetModelMatrix() * glm::vec4(0.5f * lengths.x, -0.5f * lengths.y,  0.5f * lengths.z, 1.0));
	v2 = glm::vec3(transform->GetModelMatrix() * glm::vec4(0.5f * lengths.x, -0.5f * lengths.y, -0.5f * lengths.z, 1.0));
	v3 = glm::vec3(transform->GetModelMatrix() * glm::vec4(0.5f * lengths.x,  0.5f * lengths.y, -0.5f * lengths.z, 1.0));

	
	d1 = maxDistance;
	d2 = maxDistance;
	if (glm::dot(dir, normal) < 0 
    && (glm::intersectRayTriangle(origin, dir, v0, v1, v2, bary1, d1)
	||  glm::intersectRayTriangle(origin, dir, v2, v3, v0, bary2, d2)))	
	{
		d = std::min(d1, d2);
		if (d <= 0.0001f)
		{
			d = maxDistance;
		}
		else
		{
			hitInfo.point = origin + dir * d;
			hitInfo.normal = normal;
			hitInfo.distance = d;
			hitInfo.gameObject = GetOwner().get();

			return true;
		}
	}

	// -Z
	normal = glm::vec3(transform->GetModelMatrix() * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
    normal -= GetOwner()->GetComponent<cmp::Transform>()->GetPosition();
    normal = glm::normalize(normal);


	v0 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x,  0.5f * lengths.y, -0.5f * lengths.z, 1.0));
	v1 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, -0.5f * lengths.y, -0.5f * lengths.z, 1.0));
	v2 = glm::vec3(transform->GetModelMatrix() * glm::vec4( 0.5f * lengths.x, -0.5f * lengths.y, -0.5f * lengths.z, 1.0));
	v3 = glm::vec3(transform->GetModelMatrix() * glm::vec4( 0.5f * lengths.x,  0.5f * lengths.y, -0.5f * lengths.z, 1.0));

	
	d1 = maxDistance;
	d2 = maxDistance;
	if (glm::dot(dir, normal) < 0 
    && (glm::intersectRayTriangle(origin, dir, v0, v1, v2, bary1, d1)
	||  glm::intersectRayTriangle(origin, dir, v2, v3, v0, bary2, d2)))	
	{
		d = std::min(d1, d2);
		if (d <= 0.0001f)
		{
			d = maxDistance;
		}
		else
		{
			hitInfo.point = origin + dir * d;
			hitInfo.normal = normal;
			hitInfo.distance = d;
			hitInfo.gameObject = GetOwner().get();

			return true;
		}
	}

	// +Z
	normal = glm::vec3(transform->GetModelMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    normal -= GetOwner()->GetComponent<cmp::Transform>()->GetPosition();
    normal = glm::normalize(normal);


	v0 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x,  0.5f * lengths.y, 0.5f * lengths.z, 1.0));
	v1 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, -0.5f * lengths.y, 0.5f * lengths.z, 1.0));
	v2 = glm::vec3(transform->GetModelMatrix() * glm::vec4( 0.5f * lengths.x, -0.5f * lengths.y, 0.5f * lengths.z, 1.0));
	v3 = glm::vec3(transform->GetModelMatrix() * glm::vec4( 0.5f * lengths.x,  0.5f * lengths.y, 0.5f * lengths.z, 1.0));

	
	d1 = maxDistance;
	d2 = maxDistance;
	if (glm::dot(dir, normal) < 0 
    && (glm::intersectRayTriangle(origin, dir, v0, v1, v2, bary1, d1)
	||  glm::intersectRayTriangle(origin, dir, v2, v3, v0, bary2, d2)))	
	{
		d = std::min(d1, d2);
		if (d <= 0.0001f)
		{
			d = maxDistance;
		}
		else
		{
			hitInfo.point = origin + dir * d;
			hitInfo.normal = normal;
			hitInfo.distance = d;
			hitInfo.gameObject = GetOwner().get();

			return true;
		}
	}

	// -Y
	normal = glm::vec3(transform->GetModelMatrix() * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
    normal -= GetOwner()->GetComponent<cmp::Transform>()->GetPosition();
    normal = glm::normalize(normal);


	v0 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, -0.5f * lengths.y,  0.5f * lengths.z, 1.0));
	v1 = glm::vec3(transform->GetModelMatrix() * glm::vec4( 0.5f * lengths.x, -0.5f * lengths.y,  0.5f * lengths.z, 1.0));
	v2 = glm::vec3(transform->GetModelMatrix() * glm::vec4( 0.5f * lengths.x, -0.5f * lengths.y, -0.5f * lengths.z, 1.0));
	v3 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, -0.5f * lengths.y, -0.5f * lengths.z, 1.0));

	
	d1 = maxDistance;
	d2 = maxDistance;
	if (glm::dot(dir, normal) < 0 
    && (glm::intersectRayTriangle(origin, dir, v0, v1, v2, bary1, d1)
	||  glm::intersectRayTriangle(origin, dir, v2, v3, v0, bary2, d2)))	
	{
		d = std::min(d1, d2);
		if (d <= 0.0001f)
		{
			d = maxDistance;
		}
		else
		{
			hitInfo.point = origin + dir * d;
			hitInfo.normal = normal;
			hitInfo.distance = d;
			hitInfo.gameObject = GetOwner().get();

			return true;
		}
	}

	// +Y
	normal = glm::vec3(transform->GetModelMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    normal -= GetOwner()->GetComponent<cmp::Transform>()->GetPosition();
    normal = glm::normalize(normal);


	v0 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, 0.5f * lengths.y,  0.5f * lengths.z, 1.0));
	v1 = glm::vec3(transform->GetModelMatrix() * glm::vec4( 0.5f * lengths.x, 0.5f * lengths.y,  0.5f * lengths.z, 1.0));
	v2 = glm::vec3(transform->GetModelMatrix() * glm::vec4( 0.5f * lengths.x, 0.5f * lengths.y, -0.5f * lengths.z, 1.0));
	v3 = glm::vec3(transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, 0.5f * lengths.y, -0.5f * lengths.z, 1.0));

	
	d1 = maxDistance;
	d2 = maxDistance;
	if (glm::dot(dir, normal) < 0 
    && (glm::intersectRayTriangle(origin, dir, v0, v1, v2, bary1, d1)
	||  glm::intersectRayTriangle(origin, dir, v2, v3, v0, bary2, d2)))	
	{
		d = std::min(d1, d2);
		if (d <= 0.0001f)
		{
			d = maxDistance;
		}
		else
		{
			hitInfo.point = origin + dir * d;
			hitInfo.normal = normal;
			hitInfo.distance = d;
			hitInfo.gameObject = GetOwner().get();

			return true;
		}
	}




	return false;
	// auto transform = GetOwner()->GetComponent<cmp::Transform>();
	// glm::vec3 normal(transform->GetModelMatrix() * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
    // normal -= GetOwner()->GetComponent<cmp::Transform>()->GetPosition();
    // normal = glm::normalize(normal);


	// glm::vec4 v0_4 = transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x,  0.5f * lengths.y,  0.5f * lengths.z, 1.0);
	// glm::vec4 v1_4 = transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, -0.5f * lengths.y,  0.5f * lengths.z, 1.0);
	// glm::vec4 v2_4 = transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x, -0.5f * lengths.y, -0.5f * lengths.z, 1.0);
	// glm::vec4 v4_4 = transform->GetModelMatrix() * glm::vec4(-0.5f * lengths.x,  0.5f * lengths.y, -0.5f * lengths.z, 1.0);

	// glm::vec3 v0(v0_4);
	// glm::vec3 v1(v1_4);
	// glm::vec3 v2(v2_4);
	// glm::vec3 v4(v4_4);

	// glm::vec2 bary1, bary2;
	
	// float d1 = maxDistance;
	// float d2 = maxDistance;
	// float d  = maxDistance;
	// if (glm::intersectRayTriangle(origin, dir, v0, v1, v2, bary1, d1)
	// 	|  glm::intersectRayTriangle(origin, dir, v2, v4, v0, bary2, d2))
	// {
	// 	d = std::min(d1, d2);
	// 	if (d <= 0.0001f)
	// 	{
	// 		return false;
	// 	}

	// 	hitInfo.point = origin + dir * d;
	// 	hitInfo.normal = normal;
	// 	hitInfo.distance = d;
	// 	hitInfo.gameObject = GetOwner().get();
		

	// 	return true;
	// }
	
	// return false;
}