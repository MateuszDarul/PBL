#include "TakeDamageState.h"

void TakeDamageState::Enter(std::shared_ptr<GameObject> gameObject)
{

}

void TakeDamageState::Execute(std::shared_ptr<GameObject> gameObject, float dt)
{

}

void TakeDamageState::Exit(std::shared_ptr<GameObject> gameObject)
{

}

TakeDamageState* TakeDamageState::Instance()
{
	static TakeDamageState instance;

	return &instance;
}