#include "TakeDamageState.h"
#include "FollowPathState.h"
#include "EnemyScript.h"

void TakeDamageState::Enter(std::shared_ptr<GameObject> gameObject)
{
	std::cout << "started taking damage" << std::endl;
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFleeTarget(
		gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Transform>()->GetPosition()
	);
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFlee(true);
}

void TakeDamageState::Execute(std::shared_ptr<GameObject> gameObject, float dt)
{
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFleeTarget(
		gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Transform>()->GetPosition()
	);
}

void TakeDamageState::Exit(std::shared_ptr<GameObject> gameObject)
{
	std::cout << "stopped taking damage" << std::endl;
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFlee(false);
}

TakeDamageState* TakeDamageState::Instance()
{
	static TakeDamageState instance;

	return &instance;
}