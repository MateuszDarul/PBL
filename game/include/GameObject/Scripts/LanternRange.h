#pragma once

#include "Components.h"

class Turret;
class SoundPlayer;

class LanternRange : public Script
{
public:
	std::vector<Turret*> turretsInRange;

	GameObject* playerInRange;
	
	bool isAlwaysLit = false;

	CollidersManager* colMan;
	int lineOfSightIgnoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::ENEMY | CollisionLayer::Ignore);

private:

	bool isTurnedOn = false;
	SoundPlayer* buzzingSFX = nullptr;

public:
	void Start() override;
	void Update(float dt) override;

	void ChangeLightPower(bool enabled);
	
	bool IsInRange(Turret* turret);
	bool HasLineOfSight(Turret* turret);

	void TriggerEnter(std::shared_ptr<ColliderComponent> collider) override;
	void TriggerExit(std::shared_ptr<ColliderComponent> collider) override;

	~LanternRange();
};