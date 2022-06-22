#include "Scripts/LanternRange.h"
#include "Scripts/TurretLaser.h"
#include "Scripts/TurretShoot.h"
#include "Scripts/MultiToolController.h"

#include "SceneNode.h"
#include "SoundPlayer.h"

#include <glm/gtx/rotate_vector.hpp>

void LanternRange::Start()
{
	if (!buzzingSFX) buzzingSFX = new SoundPlayer("Resources/sounds/buzzing_mono.wav");
	buzzingSFX->SetLooping(true);
	isTurnedOn = isAlwaysLit;
}

LanternRange::~LanternRange()
{
	delete buzzingSFX;
}

void LanternRange::Update(float dt)
{
	if (playerInRange && isTurnedOn)
	{
		glm::vec3 playerPos = playerInRange->GetComponent<cmp::Transform>()->GetPosition();
		float playerRot = glm::radians(playerInRange->GetComponent<cmp::Camera>()->GetYaw() - 90.0f);
		glm::vec3 thisPos = gameObject->GetNode()->GetGlobalPosition();
		glm::vec3 pos = playerPos - thisPos;
		pos = glm::rotate(pos, playerRot, glm::vec3(0.0f, 1.0f, 0.0f));
		buzzingSFX->SetPosition(pos.x, pos.y, pos.z);
	}
}

void LanternRange::ChangeLightPower(bool enabled)
{
	isTurnedOn = enabled | isAlwaysLit;
	if (buzzingSFX)
	{
		if (isTurnedOn && playerInRange) buzzingSFX->Play();
		else buzzingSFX->Stop();
	}

	for (auto turret : turretsInRange)
	{
		turret->lightSourcesInRange += (isTurnedOn/* && HasLineOfSight(turret)*/) ? 1 : -1;
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

// bool LanternRange::HasLineOfSight(Turret* turret)
// {
// 	glm::vec3 origin = gameObject->GetNode()->GetGlobalPosition();
// 	glm::vec3 target = turret->gameObject->GetNode()->GetGlobalPosition();
// 	target.y += 2.0f;
// 	glm::vec3 dir = glm::normalize(target - origin);

// 	RayHitInfo hit;
// 	while (colMan->Raycast(origin, dir, hit, 25.0f, true, lineOfSightIgnoreLayerMask))
// 	{
// 		if (turret->gameObject->Is(hit.gameObject))
// 			return true;

// 		if (auto scriptable = hit.gameObject->GetComponent<cmp::Scriptable>())
// 		{
// 			if (auto turret = scriptable->Get<Turret>())
// 			{
// 				origin = hit.point + dir * 0.1f;
// 				continue;
// 			}
// 		}

// 		return false;
// 	}

// 	return true;

// 	// printf("testing line of sight. origin: %f %f %f\ttarget: %f %f %f\n", origin.x, origin.y, origin.z, target.x, target.y, target.z);
// 	// printf("tgo: %i hgo: %i\n", turret->gameObject.get(), hit.gameObject.get());

// 	// auto namecmp = hit.gameObject->GetComponent<cmp::Name>();
// 	// std::string name = (namecmp) ? namecmp->Get() : "Unnamed";
// 	// printf("Hit go: %s \td: %f\n", name.c_str(), hit.distance);
// }

void LanternRange::TriggerEnter(std::shared_ptr<ColliderComponent> collider)
{
	if (collider->layer == CollisionLayer::Player && !playerInRange)
	{
		playerInRange = collider->GetOwner().get();
		if (isTurnedOn)
		{
			playerInRange->GetNode()->GetRoot()->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<MultiToolController>()->lightSourcesInRange += 1;
			buzzingSFX->Play();
		}
		return;
	}

	auto scriptable = collider->GetOwner()->GetComponent<cmp::Scriptable>();
	if (scriptable)
	{
		Turret* turret = scriptable->Get<Turret>();
		if (turret)
		{
			printf("Adding turret to range\n");
			// bool hasSight = HasLineOfSight(turret);
			// printf("Has %sline of sight\n", hasSight ? "" : "no ");
			// if (isTurnedOn && hasSight) turret->lightSourcesInRange += 1;
			
			if (isTurnedOn) turret->lightSourcesInRange += 1;
			turretsInRange.push_back(turret);
		}
	}
}
void LanternRange::TriggerExit(std::shared_ptr<ColliderComponent> collider)
{
	if (collider->layer == CollisionLayer::Player && playerInRange)
	{
		if (isTurnedOn)
		{
			playerInRange->GetNode()->GetRoot()->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<MultiToolController>()->lightSourcesInRange -= 1;
			buzzingSFX->FadeOut(1.0f);
		}
		playerInRange = nullptr;
		return;
	}

	auto scriptable = collider->GetOwner()->GetComponent<cmp::Scriptable>();
	if (scriptable)
	{
		Turret* turret = scriptable->Get<Turret>();
		if (turret)
		{
			if (isTurnedOn/* && HasLineOfSight(turret)*/) turret->lightSourcesInRange -= 1;
			turret->lightSourcesInRange = std::max(0, turret->lightSourcesInRange);
			turretsInRange.erase(std::remove(turretsInRange.begin(), turretsInRange.end(), turret));
		}
	}
}