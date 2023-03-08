#pragma once
#include "Flora.h"
#include "ParticleSystem.h"

class Sandbox2D : public Flora::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttatch() override;
	virtual void OnDetatch() override;
	void OnUpdate(Flora::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Flora::Event& e) override;
private:
	Flora::OrthographicCameraController m_CameraController;

	//temp 
	Flora::Ref<Flora::VertexArray> m_SquareVA;
	Flora::Ref<Flora::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	Flora::Ref<Flora::Texture2D> m_CheckerboardTexture;
	Flora::Ref<Flora::Texture2D> m_DumbTexture;

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;

	glm::vec4 m_ParticleStartColor = { 255 / 255.0f, 255 / 255.0f, 0 / 143.0f, 1.0f };
	glm::vec4 m_ParticleEndColor = { 255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 0.0f };
	float m_ParticleLifetime = 0.5f;
};