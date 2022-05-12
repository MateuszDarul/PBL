#include "FollowPathState.h"
#include "EnemyScript.h"

void FollowPathState::Enter(std::shared_ptr<GameObject> gameObject)
{
	std::cout << "Following Path" << std::endl;
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFollowPath(true);
	//gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeek(true);
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFlee(true);
}

void FollowPathState::Execute(std::shared_ptr<GameObject> gameObject, float dt)
{
	//gameObject->GetComponent<TransformComponent>()->Move(0.0f, 0.0f, dt * 5.0f);
	//gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeekTarget();
}

void FollowPathState::Exit(std::shared_ptr<GameObject> gameObject)
{
	std::cout << "Stopping following path" << std::endl;
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFollowPath(false);
	//gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetSeek(false);
	gameObject->GetComponent<ScriptComponent>()->Get<EnemyScript>()->GetSteering()->SetFlee(false);
}

FollowPathState* FollowPathState::Instance()
{
	static FollowPathState instance;

	return &instance;
}