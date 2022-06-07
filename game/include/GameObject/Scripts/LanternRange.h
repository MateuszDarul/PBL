#pragma once

#include "Components.h"

class TurretLaser;


class LanternRange : public Script
{
public:
	std::vector<TurretLaser*> turretsInRange;
	
	bool isAlwaysLit = false;
private:
	bool isTurnedOn = false;

public:
	void Start() override;
	void Update(float dt) override;

	void ChangeLightPower(bool enabled);
	
	bool IsInRange(TurretLaser* laser);

	void TriggerEnter(std::shared_ptr<ColliderComponent> collider) override;
	void TriggerExit(std::shared_ptr<ColliderComponent> collider) override;
};