#include "IdleState.h"

void IdleState::Enter(std::shared_ptr<GameObject> gameObject)
{

}

void IdleState::Execute(std::shared_ptr<GameObject> gameObject, float dt)
{

}

void IdleState::Exit(std::shared_ptr<GameObject> gameObject)
{

}

IdleState* IdleState::Instance()
{
	static IdleState instance;

	return &instance;
}