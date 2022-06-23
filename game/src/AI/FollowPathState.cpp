#include "FollowPathState.h"
#include "EnemyScript.h"
#include "Components.h"
#include "AttackingPlayerState.h"

void FollowPathState::Enter(std::shared_ptr<GameObject> gameObject)
{
	std::cout << "Following Path" << std::endl;
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFollowPath(true);
}

void FollowPathState::Execute(std::shared_ptr<GameObject> gameObject, float dt)
{
	glm::vec3 playerPos = gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Transform>()->GetPosition();

	if (glm::length(gameObject->GetComponent<cmp::Transform>()->GetPosition() - playerPos) < 10.0f)
	{
		gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->stateMachine->ChangeState(AttackingPlayerState::Instance());
	}

}

void FollowPathState::Exit(std::shared_ptr<GameObject> gameObject)
{
	std::cout << "Stopping following path" << std::endl;
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFollowPath(false);
}

FollowPathState* FollowPathState::Instance()
{
	static FollowPathState instance;

	return &instance;
}