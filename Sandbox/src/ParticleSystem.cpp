#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include <random>
#include <cstdlib>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

ParticleSystem::ParticleSystem(uint32_t maxParticles) : m_PoolIndex(maxParticles - 1)
{
	m_ParticlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(Flora::Timestep ts)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		float dir = -1.0f;
		if (rand() % 10 >= 5) {
			particle.Velocity.x *= dir;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRender(Flora::OrthographicCamera& camera)
{
	Flora::Renderer2D::BeginScene(camera);
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		glm::vec3 pos = {particle.Position.x, particle.Position.y, 0.2f};
		Flora::Renderer2D::DrawQuad(pos, { size, size }, nullptr, color, particle.Rotation);
	}
	Flora::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Position.x += particleProps.PositionVariation.x * (Random::Float() - 0.5f);
	particle.Position.y += particleProps.PositionVariation.y * (Random::Float() - 0.5f);
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorBegin.x += particleProps.ColorVariation.x * (Random::Float() - 0.5f);
	particle.ColorBegin.y += particleProps.ColorVariation.y * (Random::Float() - 0.5f);
	particle.ColorBegin.z += particleProps.ColorVariation.z * (Random::Float() - 0.5f);
	particle.ColorBegin.w += particleProps.ColorVariation.w * (Random::Float() - 0.5f);
	particle.ColorEnd = particleProps.ColorEnd;
	particle.ColorEnd.x += particleProps.ColorVariation.x * (Random::Float() - 0.5f);
	particle.ColorEnd.y += particleProps.ColorVariation.y * (Random::Float() - 0.5f);
	particle.ColorEnd.z += particleProps.ColorVariation.z * (Random::Float() - 0.5f);
	particle.ColorEnd.w += particleProps.ColorVariation.w * (Random::Float() - 0.5f);

	particle.LifeTime = particleProps.LifeTime + particleProps.LifeTimeVariation * (Random::Float() - 0.5f);
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
