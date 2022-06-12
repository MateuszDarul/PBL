#pragma once

#include <glm/vec3.hpp>
class GameObject;

enum CollisionLayer     //tu mozna dodawac kolejne warstwy w miare potrzeb
{
	Default		 = 0b0000000000000001,
	Player 		 = 0b0000000000000010,
	GUI			 = 0b0000000000000100,
	ENEMY		 = 0b0000000000001000,
	Ignore		 = 0b0000000000010000,
	Mirror		 = 0b0000000000100000,
	Light		 = 0b0000000000100001,
	LightTrigger = 0b0000000000100010,
};

struct RayHitInfo
{
	glm::vec3 point;		//!< Punkt kolizji promienia
	glm::vec3 normal;		//!< Wektor normalny trafionego kolidera
	float distance;			//!< Odleglosc punktu zderzenia od poczatku promienia
	std::shared_ptr<GameObject> gameObject;	//!< Trafiony GameObject
	CollisionLayer layer;
};