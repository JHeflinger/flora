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
	}

	{
		FL_PROFILE_SCOPE("Renderer Draw");
		Flora::Renderer2D::BeginScene(m_CameraController.GetCamera());

		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f },
			{ 0.8f, 0.8f },
			{ 0.8f, 0.2f, 0.3f, 1.0f },
			nullptr,
			rotation,
			 1.0f);

		Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f },
			{ 0.5f, 0.5f },
			{ 0.8f, 0.2f, 0.3f, 1.0f },
			m_CheckerboardTexture,
			rotation,
			10.0f);

		Flora::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	FL_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Flora::Event& e) {
	m_CameraController.OnEvent(e);
}