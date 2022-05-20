#pragma once

#include <glm/vec3.hpp>
class GameObject;

struct RayHitInfo
{
	glm::vec3 point;
	glm::vec3 normal;
	float distance;
	GameObject* gameObject;
};

enum CollisionLayer     //tu mozna dodawac kolejne warstwy w miare potrzeb
{
	Default	= 0b0000000000000001,
	Player 	= 0b0000000000000010,
	GUI		= 0b0000000000000100
};