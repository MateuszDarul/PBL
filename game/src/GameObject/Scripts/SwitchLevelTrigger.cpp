#include "Scripts/SwitchLevelTrigger.h"
#include "Scene.h"

void SwitchLevelTrigger::TriggerEnter(std::shared_ptr<ColliderComponent> other)
{
    if (other->layer & CollisionLayer::Player)
    {
        Scene::GetSceneInfo().scene->SafeSwitchLevel(newLevelIndex);
    }
}