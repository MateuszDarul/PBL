#pragma once

#include <vector>
#include "Components.h"
#include "GameObject.h"
#include "SceneNode.h"
#include "ColliderComponent.h"
#include "SoundPlayer.h"
#include "Scene.h"
#include "Turret.h"
#include "TurretRange.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TurretShoot : public Turret
{
public:
	TurretRange* turretRange;
	std::shared_ptr<ShaderComponent> particleShader;
private:
	float shootTimer = 1.0f;
	float Angle(glm::vec3 origin, glm::vec3 other)
	{
		float toReturn = glm::acos(glm::dot(glm::vec3(1, 0, 0), glm::normalize(other - origin)));
		if (origin.z - other.z <0)
		{
			toReturn = -toReturn;
		}
		return toReturn;
	}

	std::shared_ptr<ParticleComponent> muzzleParticles;

	SoundPlayer* shootSFX;

public:
	float damage = 10.0f;
	float shootCd = 1.0f;
	bool isPut = false;

	void Start()
	{
		if (muzzleParticles == nullptr)
		{
			muzzleParticles = std::make_shared<ParticleComponent>();
			
			muzzleParticles->Create(gameObject->GetNode()->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>(), true, 20, particleShader);
			muzzleParticles->SetTexture("Resources/textures/muzzle.png");
			muzzleParticles->SetParticlesPerSecond(20.0f);
			muzzleParticles->SetOffset(glm::vec3(2.50f, 2.3f, 0));
			muzzleParticles->SetDirection({ 0.0f, 0.4f, 0.0f });
			muzzleParticles->SetDirectionVar(15);
			muzzleParticles->SetParticleLifetime(0.18f);
			muzzleParticles->SetScale(0.70f, 0.75f);
			muzzleParticles->SetSpeed(0.0f);
			muzzleParticles->SetColor({ 1.0f, 1.0f, 1.0f,   1.0f }, { 1.0f, 1.0f, 1.0f,   0.5f });
			muzzleParticles->SetForce({ 0.0f, -0.01f, 0.0f });
			muzzleParticles->Stop();

			gameObject->AddComponent(muzzleParticles);
		}

		if (!shootSFX) shootSFX = new SoundPlayer("Resources/sounds/turretshoot.wav");
	}

	~TurretShoot()
	{
		if (shootSFX) delete shootSFX;
	}

	void Update(float dt)
	{
		if (isPut && turretRange && lightSourcesInRange > 0)
		{
			if (turretRange->enemies.size() > 0)
			{
				float angle = Angle(gameObject->GetComponent<cmp::Transform>()->GetPosition(), turretRange->enemies[0]->gameObject->GetComponent<cmp::Transform>()->GetPosition());
				gameObject->GetComponent<cmp::Transform>()->SetRotation(0, glm::degrees(angle), 0);
			}

			if (shootTimer < shootCd)
			{
				shootTimer += dt;
			}
			else
			{
				if (turretRange->enemies.size() > 0)
				{
					muzzleParticles->Play();
					muzzleParticles->Burst();
					turretRange->enemies[0]->DecreaseHealth(damage);
					shootTimer = 0.0f;

					float pitch = 0.8f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.4f)));
					shootSFX->SetPitch(pitch);
					shootSFX->Play3D(gameObject->GetNode()->GetGlobalPosition());
				}
			}
		}
		else
		{
			shootTimer = 0.0f;
		}
	}
};