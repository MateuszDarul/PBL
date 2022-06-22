#pragma once

#include "Components.h"

class Turret;
class SoundPlayer;

class LanternRange : public Script
{
public:
	std::vector<Turret*> turretsInRange;
	std::vector<Turret*> turretsActivated;

	GameObject* playerInRange;
	
	bool isAlwaysLit = false;

	CollidersManager* colMan;
	int lineOfSightIgnoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::ENEMY | CollisionLayer::Ignore);

private:

	bool isTurnedOn = false;
	SoundPlayer* buzzingSFX = nullptr;
	SoundPlayer* switchSFX = nullptr;

public:
	void Start() override;
	void Update(float dt) override;

	void ChangeLightPower(bool enabled);
	
	bool IsInRange(Turret* turret);
	bool IsBeingPoweredBy(Turret* turret);
	bool IsTurnedOn() const { return isTurnedOn; }

	void TriggerEnter(std::shared_ptr<ColliderComponent> collider) override;
	void TriggerExit(std::shared_ptr<ColliderComponent> collider) override;

	~LanternRange();
};