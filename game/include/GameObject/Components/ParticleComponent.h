#ifndef __PARTICLE_COMPONENT_H__
#define __PARTICLE_COMPONENT_H__
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <vector>
#include <memory>

#include "Component.h"
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
	bool burst;
	bool bursted;
	glm::vec3 offset;
	glm::vec3 direction;
	float directionVar;
	float scaleStart, scaleEnd;
	glm::vec4 colorStart, colorEnd;
	glm::vec3 force;
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
	std::map<float, int> sortedTransforms; //second is particle index

	glm::vec4 RandomDirection();

public:
	ParticleComponent();
	~ParticleComponent();
	void Create(std::shared_ptr<CameraComponent> playerCam, bool isBurst, int maxAmount);
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
	void SetForce(const glm::vec3& force);
	void SetColor(float r, float g, float b, float a = 1.0f);
	void SetColor(const glm::vec4& color);
	void SetColor(const glm::vec4& colorStart, const glm::vec4& colorEnd);
	void SetColorStart(float r, float g, float b, float a = 1.0f);
	void SetColorStart(const glm::vec4& color);
	void SetColorEnd(float r, float g, float b, float a = 1.0f);
	void SetColorEnd(const glm::vec4& color);
	void Play();
	void Stop();
	bool IsPlaying();
	void Burst();
	void SetScale(float scaleStart, float scaleEnd = -1.0f);
	float GetScale();
};
#endif