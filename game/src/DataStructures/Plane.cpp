#include "Plane.h"

Plane::Plane()
{
    
}

Plane::~Plane()
{

}

Plane::Plane(const glm::vec3& point, const glm::vec3& direction)
{
    this->normal = glm::normalize(direction);
    this->distance = glm::dot(normal, point);
}

float Plane::Distance(const glm::vec3& point)
{
    return glm::dot(this->normal, point) - this->distance;
}