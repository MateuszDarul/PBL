#ifndef __PARTICLE_COMPONENT_H__
#define __PARTICLE_COMPONENT_H__
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <vector>
#include <memory>

#include "Component.h";
#include "Particle.h"

class ShaderComponent;
class ParticleComponent : public Component
{
private:
	std::vector<std::shared_ptr<Particle>> particles;
	float particlesPerSecond;
	float particleMaxAmount;
	float particleLifetime;
	bool isPlaying;
	glm::vec3 offset;
	glm::vec3 direction;
	float directionVar;
	float scale;
	float speed;
	float spawnTimer;
	float width;
	float height;
	std::shared_ptr<CameraComponent> playerCamera;

	std::shared_ptr<ShaderComponent> shader;
	unsigned int VBO;
	unsigned int VBOinstances;
	std::vector<glm::mat4> transformations;
	unsigned int VAO;
	unsigned int texture;
	std::map<float, glm::vec3> sortedTransforms;

	glm::vec4 RandomDirection();

public:
	ParticleComponent();
	~ParticleComponent();
	void Create(std::shared_ptr<CameraComponent> playerCam);
	void SetTexture(const std::string& path);
	void Update(float dt);
	void Draw(const glm::mat4& matrixPV);
	void SetParticlesPerSecond(float particlesPerSecond);
	float GetParticlesPerSecond();
	void SetParticleMaxAmount(float maxAmount);
	float GetParticleMaxAmount();
	void SetParticleLifetime(float lifetime);
	float GetParticleLifetime();
	void SetOffset(glm::vec3 offset);
	glm::vec3 GetOffset();
	void SetDirection(glm::vec3 direction);
	glm::vec3 GetDirection();
	void SetDirectionVar(float directionVar);
	float GetDirectionVar();
	void SetSpeed(float speed);
	float GetSpeed();
	void Play();
	void Stop();
	bool IsPlaying();
	void SetScale(float scale);
	float GetScale();
};
#endif