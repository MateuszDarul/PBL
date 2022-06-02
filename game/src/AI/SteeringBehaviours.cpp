#include "SteeringBehaviours.h"
#include "EnemyScript.h"
#include <glm/glm.hpp>

glm::vec3 SteeringBehaviours::Seek(glm::vec3 targetPos)
{
	glm::vec3 desiredVelocity = glm::normalize(targetPos - owner->GetComponent<TransformComponent>()->GetPosition()) * owner->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetMaxSpeed();

	return (desiredVelocity - owner->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetVelocity());
}

glm::vec3 SteeringBehaviours::Flee(glm::vec3 targetPos)
{
	//only flee if the target is within 'panic distance'. Work in distance squared space.
	const float panicDistance = 15.0f;
	if (glm::distance(owner->GetComponent<TransformComponent>()->GetPosition(), targetPos) > panicDistance)
	{
		return glm::vec3(0.0f);
	}

	glm::vec3 desiredVelocity = glm::normalize(owner->GetComponent<TransformComponent>()->GetPosition() - targetPos) * owner->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetMaxSpeed();

	return (desiredVelocity - owner->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetVelocity());
}

glm::vec3 SteeringBehaviours::FollowPath()
{
	//move to next target if close enough to current target (working in distance squared space)
	if (glm::distance(path->CurrentWaypoint(), owner->GetComponent<TransformComponent>()->GetPosition()) < waypointSeekDist)
	{
		path->SetNextWaypoint();
	}

	if (!path->Finished())
	{
		return Seek(path->CurrentWaypoint());
	}
	else
	{
		return Seek(path->CurrentWaypoint());
	}
}

glm::vec3 SteeringBehaviours::Calculate()
{
	steeringForce = glm::vec3(0.0f);

	glm::vec3 force;

	if (seek)
	{
		force = Seek(seekTarget) * 2.0f; //modifiable parameter

		if (!AccumulateForce(steeringForce, force)) return steeringForce;
	}

	if (flee)
	{
		force = Flee(fleeTarget) * 3.0f; //modifiable parameter

		if (!AccumulateForce(steeringForce, force)) return steeringForce;
	}

	if (followPath)
	{
		force = FollowPath() * 2.0f; //modifiable parameter

		if (!AccumulateForce(steeringForce, force)) return steeringForce;
	}

	return steeringForce;
}

bool SteeringBehaviours::AccumulateForce(glm::vec3& RunningTot, glm::vec3 ForceToAdd)
{
	//calculate how much steering force the vehicle has used so far
	float MagnitudeSoFar = glm::length(RunningTot);
	//calculate how much steering force remains to be used by this vehicle
	float MagnitudeRemaining = owner->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetMaxForce() - MagnitudeSoFar;
	//return false if there is no more force left to use
	if (MagnitudeRemaining <= 0.0) return false;
	//calculate the magnitude of the force we want to add
	float MagnitudeToAdd = glm::length(ForceToAdd);
	//if the magnitude of the sum of ForceToAdd and the running total
	//does not exceed the maximum force available to this vehicle, just
	//add together. Otherwise add as much of the ForceToAdd vector as
	//possible without going over the max.
	if (MagnitudeToAdd < MagnitudeRemaining)
	{
		RunningTot += ForceToAdd;
	}
	else
	{
		//add it to the steering force
		RunningTot += (glm::normalize(ForceToAdd) * MagnitudeRemaining);
	}
	return true;
}