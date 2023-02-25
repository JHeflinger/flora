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
	m_SquareVA = Flora::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f
	};

	Flora::Ref<Flora::VertexBuffer> squareVB;
	squareVB.reset((Flora::VertexBuffer::Create(squareVertices, sizeof(squareVertices))));
	squareVB->SetLayout({
		{ Flora::ShaderDataType::Float3, "a_Position" }
	});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Flora::Ref<Flora::IndexBuffer> squareIB;
	squareIB.reset(Flora::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Flora::Shader::Create("assets/shaders/Flat.glsl");
}

void Sandbox2D::OnDetatch() {

}

void Sandbox2D::OnUpdate(Flora::Timestep ts) {
	m_CameraController.OnUpdate(ts);

	Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Flora::RenderCommand::Clear();

	Flora::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Flora::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Flora::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Flora::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Flora::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Flora::Event& e) {
	m_CameraController.OnEvent(e);
}