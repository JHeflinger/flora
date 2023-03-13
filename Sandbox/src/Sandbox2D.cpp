#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() 
	: Layer("Example"), 
	m_CameraController(1280.0f / 720.0f, true) {

}

void Sandbox2D::OnAttatch() {
	m_CheckerboardTexture = Flora::Texture2D::Create("assets/textures/test.png");
	m_DumbTexture = Flora::Texture2D::Create("assets/textures/testalpha.png");
	m_SpriteSheet = Flora::Texture2D::Create("assets/game/textures/tilemap.png");

	m_Particle.ColorBegin = { 255 / 255.0f, 255 / 255.0f, 0 / 143.0f, 1.0f };
	m_Particle.ColorEnd = { 255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 0.0f };
	m_Particle.ColorVariation = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 0.75f;
	m_Particle.LifeTimeVariation = 0.0f;
	m_Particle.Velocity = { 0.0f, 6.0f };
	m_Particle.VelocityVariation = { 6.0f, 3.0f };
	m_Particle.Position = { 0.0f, 0.0f };
	m_Particle.PositionVariation = { 0.5f, 0.5f };
}

void Sandbox2D::OnDetatch() {

}

void Sandbox2D::OnUpdate(Flora::Timestep ts) {
	FL_PROFILE_FUNCTION();

	{
		FL_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	{
		FL_PROFILE_SCOPE("Renderer Prep");
		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();
		Flora::Renderer2D::ResetStats();
	}
	/*
	{
		FL_PROFILE_SCOPE("Renderer Draw");
		Flora::Renderer2D::BeginScene(m_CameraController.GetCamera());

		static float rotation = 0.0f;
		static float rotation2 = 0.0f;
		static float rotation3 = 0.0f;
		rotation += ts * 20.0f;
		rotation2 += ts * 190.0f;
		rotation3 -= ts * 360.0f;

		static float red = 1.0f;
		static float green = 0.0f;
		static float blue = 0.0f;
		static float colorspeed = 1.5f;

		if (red >= 1.0f && blue <= 0.0f && green < 1.0f) {
			green += ts * colorspeed;
		}
		else if (red >= 0.0f && green >= 1.0f && red > 0.0f) {
			red -= ts * colorspeed;
		}
		else if (red <= 0.0f && green >= 1.0f && blue < 1.0f) {
			blue += ts * colorspeed;
		}
		else if (blue >= 1.0f && red <= 0.0f && green > 0.0f) {
			green -= ts * colorspeed;
		}
		else if (green <= 0.0f && blue >= 1.0f && red < 1.0f) {
			red += ts * colorspeed;
		}
		else if (red >= 1.0f && green <= 0.0f && blue > 0.0f) {
			blue -= ts * colorspeed;
		}

		m_SquareColor.x = red;
		m_SquareColor.y = green;
		m_SquareColor.z = blue;

		//commented out until able to debug
		
		//15 quads
		Flora::Renderer2D::DrawQuad({0.0f, 0.0f, -0.9f}, {0.8f, 0.8f}, {m_SquareColor.y, m_SquareColor.z, m_SquareColor.x, m_SquareColor.w}, nullptr, rotation, 1.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.8f }, { 0.5f, 0.5f }, { m_SquareColor.x, m_SquareColor.y, m_SquareColor.z, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2), 5.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.7f }, { 0.6f, 0.6f }, { 1.0f - m_SquareColor.x, 1.0f - m_SquareColor.y, 1.0f - m_SquareColor.z, m_SquareColor.w }, m_DumbTexture, rotation3);

		Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.x, m_SquareColor.y, m_SquareColor.z, m_SquareColor.w }, nullptr, rotation * -3.8, 1.0f);
		Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.y, m_SquareColor.z, m_SquareColor.z, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2 * -3.8), 5.0f);
		Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.y, 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.x, m_SquareColor.w }, m_DumbTexture, rotation3 * -3.8);

		Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.x, m_SquareColor.z, m_SquareColor.y, m_SquareColor.w }, nullptr, rotation * 0.5, 1.0f);
		Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.z, m_SquareColor.y, m_SquareColor.x, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2 * 0.5), 5.0f);
		Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.y, 1.0f - m_SquareColor.x, m_SquareColor.w }, m_DumbTexture, rotation3 * 0.5);

		Flora::Renderer2D::DrawQuad({ 0.0f, -1.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.z, m_SquareColor.y, m_SquareColor.x, m_SquareColor.w }, nullptr, rotation * 8.5, 1.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, -1.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.z, m_SquareColor.x, m_SquareColor.y, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2 * 7.5), 5.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, -1.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.x, 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.y, m_SquareColor.w }, m_DumbTexture, rotation3 * 0.5);

		Flora::Renderer2D::DrawQuad({ 0.0f, 1.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.x, m_SquareColor.z, m_SquareColor.x, m_SquareColor.w }, nullptr, rotation * 9.5, 1.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 1.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.y, m_SquareColor.y, m_SquareColor.z, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2 * 2.5), 5.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 1.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.x, 1.0f - m_SquareColor.y, m_SquareColor.w }, m_DumbTexture, rotation3 * 3.5);

		Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.999f }, { 40.0f, 40.0f }, {1.0f, 1.0f, 1.0f, 1.0f}, m_CheckerboardTexture, 0.0f, 20.0f);
		float bounds = 10.0f;
		float stepsize = 0.5f;
		float spacing = 0.05f;
		for (float y = -bounds; y < bounds; y += stepsize) {
			for (float x = -bounds; x < bounds; x += stepsize) {
				glm::vec4 color = { (x + bounds) / bounds * 2, 0.4f, (y + bounds) / bounds * 2, 0.5f };
				Flora::Renderer2D::DrawQuad({ x, y, -0.99f }, { stepsize - spacing, stepsize - spacing }, color);
			}
		}
		Flora::Renderer2D::EndScene();
	}
	*/
	{
		FL_PROFILE_SCOPE("TILEMAP TEST");
		Flora::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SpriteSheet);
		Flora::Renderer2D::EndScene();
	}
	
	{
		FL_PROFILE_SCOPE("PARTICLE RENDER TEST");

		Flora::Renderer2D::BeginScene(m_CameraController.GetCamera());
		if (Flora::Input::IsMouseButtonPressed(FL_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Flora::Input::GetMousePosition();
			auto width = Flora::Application::Get().GetWindow().GetWidth();
			auto height = Flora::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 20; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
		Flora::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	FL_PROFILE_FUNCTION();

	ImGui::Begin("Renderer Stats");
	auto stats = Flora::Renderer2D::GetStats();
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();

	//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	/*ImGui::Begin("Particle Settings");
	ImGui::SliderFloat("X Variation", &m_Particle.PositionVariation.x, 0, 10, "%.3f", 0);
	ImGui::SliderFloat("Y Variation", &m_Particle.PositionVariation.y, 0, 10, "%.3f", 0);
	ImGui::SliderFloat("X Velocity", &m_Particle.Velocity.x, 0, 100, "%.3f", 0);
	ImGui::SliderFloat("Y Velocity", &m_Particle.Velocity.y, 0, 100, "%.3f", 0);
	ImGui::SliderFloat("X Velocity Variation", &m_Particle.VelocityVariation.x, 0, 100, "%.3f", 0);
	ImGui::SliderFloat("Y Velocity Variation", &m_Particle.VelocityVariation.y, 0, 100, "%.3f", 0);
	ImGui::SliderFloat("RED Variation", &m_Particle.ColorVariation.x, 0, 10, "%.3f", 0);
	ImGui::SliderFloat("GREEN Variation", &m_Particle.ColorVariation.y, 0, 1, "%.3f", 0);
	ImGui::SliderFloat("BLUE Variation", &m_Particle.ColorVariation.z, 0, 1, "%.3f", 0);
	ImGui::SliderFloat("ALPHA Variation", &m_Particle.ColorVariation.w, 0, 1, "%.3f", 0);
	ImGui::SliderFloat("Size Variation", &m_Particle.SizeVariation, 0, 20, "%.3f", 0);
	ImGui::SliderFloat("Start Size", &m_Particle.SizeBegin, 0, 20, "%.3f", 0);
	ImGui::SliderFloat("End Size", &m_Particle.SizeEnd, 0, 20, "%.3f", 0);
	ImGui::SliderFloat("Lifetime", &m_Particle.LifeTime, 0, 2, "%.3f", 0);
	ImGui::SliderFloat("Lifetime Variation", &m_Particle.LifeTimeVariation, 0, 2, "%.3f", 0);
	ImGui::End();*/
}

void Sandbox2D::OnEvent(Flora::Event& e) {
	m_CameraController.OnEvent(e);
}