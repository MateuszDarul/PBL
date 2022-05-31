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

	if (glm::length(velocity) > 0.0000001f)
	{
		heading = glm::normalize(velocity);
	}

	//check if attacked by flashlight
	glm::vec3 fl_pos = player->GetComponent<CameraComponent>()->GetPosition();
	glm::vec3 fl_forward = player->GetComponent<CameraComponent>()->GetForward();
	glm::vec3 mypos = gameObject->GetComponent<TransformComponent>()->GetPosition();

	float distance = glm::length(fl_pos - mypos);
	float dot = glm::dot(glm::normalize(mypos - fl_pos), glm::normalize(fl_forward));

	//std::cout << fl_pos.x << std::endl;
	//std::cout << fl_forward.x << std::endl;
	//std::cout << mypos.x << std::endl;
	//std::cout << dot << std::endl;

	if (multitool->isFlashlightOn  && dot > 0.7f && distance < 16.66887f)
	{
		// player looks at me
		std::cout << "I am taking dmg" << std::endl;
		health->DecreaseHealth(2.0f * dt);
	}

}
