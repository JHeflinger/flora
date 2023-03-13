#pragma once

#include <Flora.h>

struct ParticleProps
{
	glm::vec2 Position, PositionVariation;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	glm::vec4 ColorVariation;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
	float LifeTimeVariation = 0.0f;
};

class ParticleSystem
{
public:
	ParticleSystem(uint32_t maxParticles = 100000);

	void OnUpdate(Flora::Timestep ts);
	void OnRender(Flora::OrthographicCamera& camera);

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex;
};