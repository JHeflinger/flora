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
	m_DumbTexture = Flora::Texture2D::Create("assets/textures/test.png");
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

		//15 quads
		//Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.9f }, { 0.8f, 0.8f }, { m_SquareColor.y, m_SquareColor.z, m_SquareColor.x, m_SquareColor.w }, nullptr, rotation, 1.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.8f }, { 0.5f, 0.5f }, { m_SquareColor.x, m_SquareColor.y, m_SquareColor.z, m_SquareColor.w }, m_CheckerboardTexture, rotation2, 5.0f);
		//Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.7f }, { 0.6f, 0.6f }, { 1.0f - m_SquareColor.x, 1.0f - m_SquareColor.y, 1.0f - m_SquareColor.z, m_SquareColor.w }, m_DumbTexture, rotation3);

		//Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.x, m_SquareColor.y, m_SquareColor.z, m_SquareColor.w }, nullptr, rotation * -3.8, 1.0f);
		Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.y, m_SquareColor.z, m_SquareColor.z, m_SquareColor.w }, m_CheckerboardTexture, rotation2 * -3.8, 5.0f);
		//Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.y, 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.x, m_SquareColor.w }, m_DumbTexture, rotation3 * -3.8);

		//Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.x, m_SquareColor.z, m_SquareColor.y, m_SquareColor.w }, nullptr, rotation * 0.5, 1.0f);
		Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.z, m_SquareColor.y, m_SquareColor.x, m_SquareColor.w }, m_CheckerboardTexture, rotation2 * 0.5, 5.0f);
		//Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.y, 1.0f - m_SquareColor.x, m_SquareColor.w }, m_DumbTexture, rotation3 * 0.5);

		//Flora::Renderer2D::DrawQuad({ 0.0f, -1.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.z, m_SquareColor.y, m_SquareColor.x, m_SquareColor.w }, nullptr, rotation * 8.5, 1.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, -1.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.z, m_SquareColor.x, m_SquareColor.y, m_SquareColor.w }, m_CheckerboardTexture, rotation2 * 7.5, 5.0f);
		//Flora::Renderer2D::DrawQuad({ 0.0f, -1.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.x, 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.y, m_SquareColor.w }, m_DumbTexture, rotation3 * 0.5);

		//Flora::Renderer2D::DrawQuad({ 0.0f, 1.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.x, m_SquareColor.z, m_SquareColor.x, m_SquareColor.w }, nullptr, rotation * 9.5, 1.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 1.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.y, m_SquareColor.y, m_SquareColor.z, m_SquareColor.w }, m_CheckerboardTexture, rotation2 * 2.5, 5.0f);
		//Flora::Renderer2D::DrawQuad({ 0.0f, 1.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.x, 1.0f - m_SquareColor.y, m_SquareColor.w }, m_DumbTexture, rotation3 * 3.5);

		float bounds = 10.0f;
		float stepsize = 0.5f;
		float spacing = 0.05f;
		for (float y = -bounds; y < bounds; y += stepsize) {
			for (float x = -bounds; x < bounds; x += stepsize) {
				glm::vec4 color = { (x + bounds) / bounds * 2, 0.4f, (y + bounds) / bounds * 2, 1.0f };
				Flora::Renderer2D::DrawQuad({ x, y, -0.99f }, { stepsize - spacing, stepsize - spacing }, color);
			}
		}
		Flora::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	FL_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Flora::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Flora::Event& e) {
	m_CameraController.OnEvent(e);
}