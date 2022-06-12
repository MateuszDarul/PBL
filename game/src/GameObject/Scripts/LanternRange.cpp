#include "Scripts/LanternRange.h"
#include "Scripts/TurretLaser.h"
#include "Scripts/TurretShoot.h"
#include "Scripts/MultiToolController.h"

#include "SceneNode.h"

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
		turret->lightSourcesInRange += (isTurnedOn && HasLineOfSight(turret)) ? 1 : -1;
	}
	if (playerInRange)
	{
		playerInRange->GetNode()->GetRoot()->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<MultiToolController>()->lightSourcesInRange += isTurnedOn ? 1 : -1;
	}
}

bool LanternRange::IsInRange(Turret* turret)
{
	return std::find(turretsInRange.begin(), turretsInRange.end(), turret) != turretsInRange.end();
}

bool LanternRange::HasLineOfSight(Turret* turret)
{
	glm::vec3 origin = gameObject->GetNode()->GetGlobalPosition();
	glm::vec3 target = turret->gameObject->GetNode()->GetGlobalPosition();
	target.y += 2.0f;
	glm::vec3 dir = glm::normalize(target - origin);

	RayHitInfo hit;
	while (colMan->Raycast(origin, dir, hit, 25.0f, true, lineOfSightIgnoreLayerMask))
	{
		if (turret->gameObject->Is(hit.gameObject))
			return true;

		if (auto scriptable = hit.gameObject->GetComponent<cmp::Scriptable>())
		{
			if (auto turret = scriptable->Get<Turret>())
			{
				origin = hit.point + dir * 0.1f;
				continue;
			}
		}

		return false;
	}

	return true;

	// printf("testing line of sight. origin: %f %f %f\ttarget: %f %f %f\n", origin.x, origin.y, origin.z, target.x, target.y, target.z);
	// printf("tgo: %i hgo: %i\n", turret->gameObject.get(), hit.gameObject.get());

	// auto namecmp = hit.gameObject->GetComponent<cmp::Name>();
	// std::string name = (namecmp) ? namecmp->Get() : "Unnamed";
	// printf("Hit go: %s \td: %f\n", name.c_str(), hit.distance);
}

void LanternRange::TriggerEnter(std::shared_ptr<ColliderComponent> collider)
{
	if (collider->layer == CollisionLayer::Player && !playerInRange)
	{
		playerInRange = collider->GetOwner().get();
		if (isTurnedOn) playerInRange->GetNode()->GetRoot()->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<MultiToolController>()->lightSourcesInRange += 1;
		return;
	}

	auto scriptable = collider->GetOwner()->GetComponent<cmp::Scriptable>();
	if (scriptable)
	{
		Turret* turret = scriptable->Get<TurretLaser>();
		if (!turret) turret = scriptable->Get<TurretShoot>();
		if (turret)
		{
			printf("Adding turret to range\n");
			bool hasSight = HasLineOfSight(turret);
			if (isTurnedOn && hasSight) turret->lightSourcesInRange += 1;
			turretsInRange.push_back(turret);

			printf("Has %sline of sight\n", hasSight ? "" : "no ");
		}
	}
}
void LanternRange::TriggerExit(std::shared_ptr<ColliderComponent> collider)
{
	if (collider->layer == CollisionLayer::Player && playerInRange)
	{
		if (isTurnedOn) playerInRange->GetNode()->GetRoot()->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<MultiToolController>()->lightSourcesInRange -= 1;
		playerInRange = nullptr;
		return;
	}

	auto scriptable = collider->GetOwner()->GetComponent<cmp::Scriptable>();
	if (scriptable)
	{
		Turret* turret = scriptable->Get<TurretLaser>();
		if (!turret) turret = scriptable->Get<TurretShoot>();
		if (turret)
		{
			if (isTurnedOn && HasLineOfSight(turret)) turret->lightSourcesInRange -= 1;
			turretsInRange.erase(std::remove(turretsInRange.begin(), turretsInRange.end(), turret));
		}
	}
}