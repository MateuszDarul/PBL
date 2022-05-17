#include <cstdlib>
#include <ctime>
#include "GameObject.h"
#include "CameraComponent.h"
#include "ParticleComponent.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"

glm::vec4 ParticleComponent::RandomDirection()
{
	float randomX = float(rand()) / float(RAND_MAX) * directionVar * 2 - directionVar;
	float randomY = float(rand()) / float(RAND_MAX) * directionVar * 2 - directionVar;
	float randomZ = float(rand()) / float(RAND_MAX) * directionVar * 2 - directionVar;
	glm::vec4 randomDir = glm::vec4(direction, 1.0) *
		glm::rotate(glm::mat4(1.f), glm::radians(randomZ), glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1.f), glm::radians(randomY), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1.f), glm::radians(randomX), glm::vec3(1, 0, 0));
	return randomDir;
}

ParticleComponent::ParticleComponent() : Component(15), particlesPerSecond(2), particleMaxAmount(10), particleLifetime(1), isPlaying(true), direction(1.0f), speed(1.0f), offset(0.0f), scale(1.0f)
{
	spawnTimer = 0;
	directionVar = 0;
}

ParticleComponent::~ParticleComponent()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->VBOinstances);
}

void ParticleComponent::Create(std::shared_ptr<CameraComponent> playerCam)
{
	srand(time(NULL));
	playerCamera = playerCam;
	for (int i = 0; i < particleMaxAmount; i++)
	{
		transformations.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));
	}
	shader = std::make_shared<ShaderComponent>();
	shader->Create("Resources/shaders/particles.vert", "Resources/shaders/particles.frag");
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glGenBuffers(1, &this->VBOinstances);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOinstances);
	glBufferData(GL_ARRAY_BUFFER, this->particleMaxAmount * sizeof(glm::mat4), &this->transformations[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);


	glBindVertexArray(0);
}

void ParticleComponent::SetTexture(const std::string& path)
{
	std::string texturesDir = path;

	std::fstream file(path.c_str(), std::ios::in);

	if (file.good())
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrComponents;
		//stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			this->width = width;
			this->height = height;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cerr << "Texture failed to load at path: " << path << std::endl;
		}
		stbi_image_free(data);
	}

	file.close();
}

void ParticleComponent::Update(float dt)
{
	if (isPlaying)
	{
		float timeBetweenParticles = 1 / particlesPerSecond;
		if (particles.size() < particleMaxAmount)
		{
			float particlesToSpawn = std::floor(spawnTimer * particlesPerSecond - particles.size());
			if (particlesToSpawn > 0)
			{
				for (int i = 0; i < particlesToSpawn; i++)
				{
					particles.push_back(std::make_shared<Particle>(glm::vec3(0.0f, 0.0f, 0.0f), RandomDirection(), speed, particleLifetime));
				}
			}
			spawnTimer += dt;
		}

		sortedTransforms.clear();
		for (int i = 0; i < particles.size(); i++)
		{
			particles[i]->UpdatePos(dt, RandomDirection());
			particles[i]->UpdateLifetime(dt);
			float distance = glm::length(playerCamera->GetPosition() - particles[i]->position);
			sortedTransforms[distance] = particles[i]->position;
		}
		int i = 0;
		for (std::map<float, glm::vec3>::reverse_iterator it = sortedTransforms.rbegin(); it != sortedTransforms.rend(); it++)
		{
			transformations[i] = glm::translate(glm::mat4(1.0f), it->second);
			i++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->VBOinstances);
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(ptr, &this->transformations[0], particleMaxAmount * sizeof(glm::mat4));
		glUnmapBuffer(GL_ARRAY_BUFFER);

	}
}

void ParticleComponent::Draw(const glm::mat4& matrixPV)
{
	if (isPlaying)
	{
		shader->Use();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		shader->SetInt("texture", 0);
		shader->SetMat4("matrixPV", matrixPV);
		shader->SetMat4("cameraView", playerCamera->GetView());
		shader->SetFloat("width", 1);
		shader->SetFloat("height", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glm::vec3 pos = offset + GetOwner()->GetComponent<TransformComponent>()->GetPosition();
		shader->SetMat4("model", glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.f), glm::vec3(scale, scale, scale)));
		glBindVertexArray(this->VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particles.size());
		glBindVertexArray(0);
		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void ParticleComponent::SetParticlesPerSecond(float particlesPerSecond)
{
	this->particlesPerSecond = particlesPerSecond;
}

float ParticleComponent::GetParticlesPerSecond()
{
	return particlesPerSecond;
}

void ParticleComponent::SetParticleMaxAmount(float maxAmount)
{
	float amountDiff = particleMaxAmount;
	particleMaxAmount = maxAmount;
	amountDiff -= particleMaxAmount;
	if (amountDiff > 0)
	{
		for (int i = 0; i < amountDiff; i++)
		{
			particles.pop_back();
			transformations.pop_back();
		}
	}
	else if (amountDiff < 0)
	{
		for (int i = 0; i < std::abs(amountDiff); i++)
		{
			transformations.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));
		}
	}

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOinstances);
	glBufferData(GL_ARRAY_BUFFER, this->particleMaxAmount * sizeof(glm::mat4), &this->transformations[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
}

float ParticleComponent::GetParticleMaxAmount()
{
	return particleMaxAmount;
}

void ParticleComponent::SetParticleLifetime(float lifetime)
{
	particleLifetime = lifetime;
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->lifetimeTimer = lifetime - particles[i]->lifetime + particles[i]->lifetimeTimer;
		particles[i]->lifetime = lifetime;
	}
}

float ParticleComponent::GetParticleLifetime()
{
	return particleLifetime;
}

void ParticleComponent::SetOffset(glm::vec3 offset)
{
	this->offset = offset;
}

glm::vec3 ParticleComponent::GetOffset()
{
	return offset;
}

void ParticleComponent::SetDirection(glm::vec3 direction)
{
	this->direction = direction;
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->direction = direction;
	}
}

glm::vec3 ParticleComponent::GetDirection()
{
	return direction;
}

void ParticleComponent::SetDirectionVar(float directionVar)
{
	this->directionVar = directionVar;
}

float ParticleComponent::GetDirectionVar()
{
	return directionVar;
}

void ParticleComponent::SetSpeed(float speed)
{
	this->speed = speed;
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->speed = speed;
	}
}

float ParticleComponent::GetSpeed()
{
	return speed;
}

void ParticleComponent::Play()
{
	isPlaying = true;
}

void ParticleComponent::Stop()
{
	isPlaying = false;
}

bool ParticleComponent::IsPlaying()
{
	return isPlaying;
}

void ParticleComponent::SetScale(float scale)
{
	this->scale = scale;
}

float ParticleComponent::GetScale()
{
	return scale;
}

