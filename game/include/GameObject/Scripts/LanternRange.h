#pragma once

#include "Components.h"

class TurretLaser;


class LanternRange : public Script
{
public:
	std::vector<TurretLaser*> turretsInRange;

	GameObject* playerInRange;
	
	bool isAlwaysLit = false;

	CollidersManager* colMan;
	int lineOfSightIgnoreLayerMask = ~(CollisionLayer::Player | CollisionLayer::Ignore);

private:

	bool isTurnedOn = false;

public:
	void Start() override;
	void Update(float dt) override;

	void ChangeLightPower(bool enabled);
	
	bool IsInRange(TurretLaser* turret);
	bool HasLineOfSight(TurretLaser* turret);

	void TriggerEnter(std::shared_ptr<ColliderComponent> collider) override;
	void TriggerExit(std::shared_ptr<ColliderComponent> collider) override;
};