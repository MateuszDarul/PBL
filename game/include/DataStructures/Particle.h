#ifndef __PARTICLE_H__
#define __PARTICLE_H__
#include <glm/glm.hpp>

class Particle
{
public:
	glm::vec3 position;
	glm::vec3 initPosition;
	glm::vec3 direction;
	float speed;
	float lifetime;
	float lifetimeTimer;
	bool burst;
	bool isDead;
	Particle(glm::vec3 position, glm::vec3 direction, float speed, float lifetime, bool burst);
	void UpdatePos(float dt, glm::vec3 direction);
	void UpdateLifetime(float dt);
};
#endif