#include "Scripts/LanternRange.h"
#include "Scripts/TurretLaser.h"

void LanternRange::Start()
{
}

void LanternRange::Update(float dt)
{
}

void LanternRange::ChangeLightPower(bool enabled)
{
	isTurnedOn = enabled | isAlwaysLit;
	for (auto turret : turretsInRange)
	{
		turret->lightSourcesInRange += isTurnedOn ? 1 : -1;
	}
}

bool LanternRange::IsInRange(TurretLaser* laser)
{
	return std::find(turretsInRange.begin(), turretsInRange.end(), laser) != turretsInRange.end();
}

void LanternRange::TriggerEnter(std::shared_ptr<ColliderComponent> collider)
{
	auto scriptable = collider->GetOwner()->GetComponent<cmp::Scriptable>();
	if (scriptable)
	{
		if (auto turret = scriptable->Get<TurretLaser>())
		{
			turret->lightSourcesInRange += 1;
			turretsInRange.push_back(turret);
		}
	}
}
void LanternRange::TriggerExit(std::shared_ptr<ColliderComponent> collider)
{
	auto scriptable = collider->GetOwner()->GetComponent<cmp::Scriptable>();
	if (scriptable)
	{
		if (auto turret = scriptable->Get<TurretLaser>())
		{
			turret->lightSourcesInRange -= 1;
			turretsInRange.erase(std::remove(turretsInRange.begin(), turretsInRange.end(), turret));
		}
	}
}