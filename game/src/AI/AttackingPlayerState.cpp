#include "AttackingPlayerState.h"
#include "EnemyScript.h"

void AttackingPlayerState::Enter(std::shared_ptr<GameObject> gameObject)
{
	std::cout << "Chasing player" << std::endl;
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeekTarget(
		gameObject->GetNode()->GetParent()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Transform>()->GetPosition()
	);
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeek(true);
}

void AttackingPlayerState::Execute(std::shared_ptr<GameObject> gameObject, float dt)
{
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeekTarget(
		gameObject->GetNode()->GetParent()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Transform>()->GetPosition()
	);
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