#include "EnemyScript.h"
#include "GameObject.h"
#include "Components.h"
#include "FollowPathState.h"

void EnemyScript::Start() 
{
	velocity = glm::vec3(0.0f);

	heading = glm::vec3(0.0f, 0.0f, 1.0f);

	side = glm::vec3(1.0f, 0.0f, 0.0f);

	top = glm::vec3(0.0f, 1.0f, 0.0f);

	maxSpeed = 2.0f;

	maxForce = 1.0f;

	maxTurnRate = 3.0f;

	//WORK IN PROGRESS
	path = new Path();

	steering = new SteeringBehaviours(gameObject, path);

	steering->SetSeekTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	steering->SetFleeTarget(glm::vec3(0.0f, 0.0f, 0.0f));

	stateMachine = std::make_shared<StateMachine<GameObject>>(gameObject);
	stateMachine->SetCurrentState(FollowPathState::Instance());
	stateMachine->ChangeState(FollowPathState::Instance());

}

void EnemyScript::Update(float dt) 
{

	stateMachine->Update(dt);

	glm::vec3 steeringForce = steering->Calculate();

	glm::vec3 acceleration = steeringForce; // steeringForce/mass if implemented

	velocity += acceleration * dt;

	steering->SetFleeTarget(player->GetComponent<CameraComponent>()->GetPosition());

	if (glm::length(velocity) > maxSpeed) 
	{
		velocity = glm::normalize(velocity) * maxSpeed;
	}

	gameObject->GetComponent<TransformComponent>()->Move(velocity * dt);

	//std::cout << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;

	//std::cout << gameObject->GetComponent<TransformComponent>()->GetPosition().x << ", " << gameObject->GetComponent<TransformComponent>()->GetPosition().y <<
	//	", " << gameObject->GetComponent<TransformComponent>()->GetPosition().z << std::endl;

	if (glm::length(velocity) > 0.0000001f)
	{
		heading = glm::normalize(velocity);
	}

}