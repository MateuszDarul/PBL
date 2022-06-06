#include "SlopeCollider.h"
#include "Components.h"
#include "GameObject.h"
#include "SceneNode.h"

#include "CollidersManager.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

SlopeCollider::SlopeCollider()
	:ColliderComponent(18, false, false, CollisionLayer::Default)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	isStatic = false;
	isTrigger = false;
	dimensions = { 1.0f, 1.0f, 1.0f };
    direction = Direction::Z;
    CalculateNormal();
}

SlopeCollider::SlopeCollider(bool isTrigger, bool isStatic, int layer)
	:ColliderComponent(18, isTrigger, isStatic, layer)
{
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	dimensions = { 1.0f, 1.0f, 1.0f };
    direction = Direction::Z;
    CalculateNormal();
}

SlopeCollider::~SlopeCollider()
{

}

void SlopeCollider::SetDimensions(float width, float height, float length)
{
    dimensions = { width, height, length };
    CalculateNormal();
}

void SlopeCollider::SetDimensions(const glm::vec3& newDimensions)
{
    dimensions = newDimensions;
    CalculateNormal();
}

const glm::vec3& SlopeCollider::GetDimensions() const
{
    return dimensions;
}

const glm::vec3& SlopeCollider::GetNormal() const
{
    return normal;
}

const glm::vec3& SlopeCollider::GetAlongX() const
{
    return alongX;
}

const glm::vec3& SlopeCollider::GetAlongZ() const
{
    return alongZ;
}

float SlopeCollider::GetLengthX() const
{
    return lenX;
}

float SlopeCollider::GetLengthZ() const
{
    return lenZ;
}

void SlopeCollider::CalculateNormal()
{
    switch (direction)
    {
        case Direction::Z:
            normal = glm::normalize(glm::vec3(0.0f, dimensions.z, -dimensions.y));
            alongZ = glm::normalize(glm::vec3(0.0f, dimensions.y,  dimensions.z));
            alongX = glm::cross(normal, alongZ);
            lenX = dimensions.x;
            lenZ = sqrt(dimensions.z * dimensions.z + dimensions.y * dimensions.y);
        break;
        case Direction::Z_NEG:
            normal = glm::normalize(glm::vec3(0.0f, dimensions.z,  dimensions.y));
            alongZ = glm::normalize(glm::vec3(0.0f, dimensions.y, -dimensions.z));
            alongX = glm::cross(normal, alongZ);
            lenX = dimensions.x;
            lenZ = sqrt(dimensions.z * dimensions.z + dimensions.y * dimensions.y);
        break;
        case Direction::X:
            normal = glm::normalize(glm::vec3(-dimensions.y, dimensions.x, 0.0f));
            alongX = glm::normalize(glm::vec3( dimensions.x, dimensions.y, 0.0f));
            alongZ = glm::cross(normal, alongX);
            lenZ = dimensions.z;
            lenX = sqrt(dimensions.x * dimensions.x + dimensions.y * dimensions.y);
        break;
        case Direction::X_NEG:
            normal = glm::normalize(glm::vec3( dimensions.y, dimensions.x, 0.0f));
            alongX = glm::normalize(glm::vec3(-dimensions.x, dimensions.y, 0.0f));
            alongZ = glm::cross(normal, alongX);
            lenZ = dimensions.z;
            lenX = sqrt(dimensions.x * dimensions.x + dimensions.y * dimensions.y);
        break;
    }
    
}

void SlopeCollider::SetDirection(Direction dir)
{
    direction = dir;
    CalculateNormal();
}

bool SlopeCollider::CheckCollision(std::shared_ptr<ColliderComponent> collider)
{	
	return false;
}

bool SlopeCollider::RayCollision(const glm::vec3& origin, const glm::vec3& dir, RayHitInfo& hitInfo, float maxDistance)
{
	if (!isStatic)
	{
		return false;
	}

	float d1, d2, d = maxDistance;
	glm::vec2 bary1, bary2;

	d1 = maxDistance;
	d2 = maxDistance;
	if (glm::dot(dir, normal) < 0)
	{
        const auto& model = GetOwner()->GetNode()->GetGlobalTransformations();
        glm::vec3 c = { model[3][0]+offset.x, model[3][1]+offset.y, model[3][2]+offset.z };

        glm::vec3 p1 = c + lenX * 0.5f * alongX + lenZ * 0.5f * alongZ;
        glm::vec3 p2 = c - lenX * 0.5f * alongX + lenZ * 0.5f * alongZ;
        glm::vec3 p3 = c - lenX * 0.5f * alongX - lenZ * 0.5f * alongZ;
        glm::vec3 p4 = c + lenX * 0.5f * alongX - lenZ * 0.5f * alongZ;
		
		if (glm::intersectRayTriangle(origin, dir, p1, p2, p3, bary1, d1)
		||  glm::intersectRayTriangle(origin, dir, p3, p1, p4, bary2, d2))
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
				hitInfo.gameObject = GetOwner();

				return true;
			}
		}
	}

	return false;
}