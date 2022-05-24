#include "Particle.h"

Particle::Particle(glm::vec3 position, glm::vec3 direction, float speed, float lifetime):position(position), initPosition(position), direction(direction), speed(speed), lifetime(lifetime), lifetimeTimer(lifetime)
{
}

void Particle::UpdatePos(float dt, glm::vec3 direction, const glm::vec3& force)
{
	if (lifetimeTimer > 0)
	{
		position += this->direction * speed * dt;
		this->direction += force * dt;
	}
	else
	{
		position = initPosition;
		this->direction = direction;
	}
}

void Particle::UpdateLifetime(float dt)
{
	if (lifetimeTimer <= 0)
	{
		lifetimeTimer = lifetime;
	}
	lifetimeTimer -= dt;
}
