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
	m_CameraController.OnUpdate(ts);

	Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Flora::RenderCommand::Clear();

	Flora::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor, 20.0f);
	Flora::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor * 0.7f, 90.0f);
	Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 5.0f, 5.0f }, m_CheckerboardTexture, { m_SquareColor.x * 0.5f, m_SquareColor.y * 0.9f, m_SquareColor.z * 0.2f, m_SquareColor.w * 0.2f }, 45.0f);

	Flora::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Flora::Event& e) {
	m_CameraController.OnEvent(e);
}