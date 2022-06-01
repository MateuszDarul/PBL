#include "AttackingTowerState.h"

void AttackingTowerState::Enter(std::shared_ptr<GameObject> gameObject)
{

}

void AttackingTowerState::Execute(std::shared_ptr<GameObject> gameObject, float dt)
{

}

void AttackingTowerState::Exit(std::shared_ptr<GameObject> gameObject)
{

}

AttackingTowerState* AttackingTowerState::Instance()
{
	static AttackingTowerState instance;

	return &instance;
}