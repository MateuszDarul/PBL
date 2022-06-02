#ifndef __STEERING_BEHAVIOURS_H__
#define __STEERING_BEHAVIOURS_H__

#include <glm/vec3.hpp>
#include <memory>
#include "GameObject.h"
#include "Path.h"

//klasa zachowań sterujących z metodami seek, followPath i wallAvoidance
class SteeringBehaviours
{
private:
	bool seek = false;
	bool flee = false;
	bool followPath = false;

	std::shared_ptr<GameObject> owner;
	std::shared_ptr<Path> path;
	glm::vec3 seekTarget;
	glm::vec3 fleeTarget;

	glm::vec3 steeringForce;

	float waypointSeekDist = 2.0f;

	glm::vec3 Seek(glm::vec3);
	glm::vec3 Flee(glm::vec3);
	glm::vec3 FollowPath();

	bool AccumulateForce(glm::vec3&, glm::vec3);

public:
	SteeringBehaviours(std::shared_ptr<GameObject> own, std::shared_ptr<Path> pth) {

		steeringForce = glm::vec3(0.0f);

		owner = own;
		path = pth;
	}

	glm::vec3 Calculate();

	void SetSeekTarget(glm::vec3 trgt) { seekTarget = trgt; }
	void SetFleeTarget(glm::vec3 trgt) { fleeTarget = trgt; }
	void SetPath(std::shared_ptr<Path> pth) { path = pth; }

	void SetSeek(bool on) { seek = on; }
	void SetFlee(bool on) { flee = on; }
	void SetFollowPath(bool on) { followPath = on; }

};

#endif