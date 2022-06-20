#include "AttackingPlayerState.h"
#include "FollowPathState.h"
#include "EnemyScript.h"

void AttackingPlayerState::Enter(std::shared_ptr<GameObject> gameObject)
{
	std::cout << "Chasing player" << std::endl;
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeekTarget(
		gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Transform>()->GetPosition()
	);
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeek(true);
}

void AttackingPlayerState::Execute(std::shared_ptr<GameObject> gameObject, float dt)
{
	glm::vec3 playerPos = gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Transform>()->GetPosition();

	if (glm::length(gameObject->GetComponent<cmp::Transform>()->GetPosition() - playerPos) > 20.0f)
	{
		gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->stateMachine->ChangeState(FollowPathState::Instance());
	}

	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeekTarget(playerPos);
}

void AttackingPlayerState::Exit(std::shared_ptr<GameObject> gameObject)
{
	std::cout << "Stopping chasing player" << std::endl;
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeek(false);
}

AttackingPlayerState* AttackingPlayerState::Instance()
{
	static AttackingPlayerState instance;

	return &instance;
}