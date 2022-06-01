#include "RetreatFromFlashlightState.h"

void RetreatFromFlashlightState::Enter(std::shared_ptr<GameObject> gameObject)
{

}

void RetreatFromFlashlightState::Execute(std::shared_ptr<GameObject> gameObject, float dt)
{

}

void RetreatFromFlashlightState::Exit(std::shared_ptr<GameObject> gameObject)
{

}

RetreatFromFlashlightState* RetreatFromFlashlightState::Instance()
{
	static RetreatFromFlashlightState instance;

	return &instance;
}