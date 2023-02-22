#include <Flora.h>
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Flora::Layer {
public:
	ExampleLayer() 
		: Layer("Example"), 
		m_CameraController(1280.0f / 720.0f, true) {

		m_SquareVA.reset(Flora::VertexArray::Create());
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.0f,  0.0f,  1.0f
		};
		Flora::Ref<Flora::VertexBuffer> squareVB;
		squareVB.reset((Flora::VertexBuffer::Create(squareVertices, sizeof(squareVertices))));
		squareVB->SetLayout({
			{ Flora::ShaderDataType::Float3, "a_Position" },
			{ Flora::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Flora::Ref<Flora::IndexBuffer> squareIB;
		squareIB.reset(Flora::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		auto squareShader = m_ShaderLibrary.Load("assets/shaders/Square.glsl");

		m_Texture = Flora::Texture2D::Create("assets/textures/test.png");
		m_AlphaTexture = Flora::Texture2D::Create("assets/textures/testalpha.png");
	
		std::dynamic_pointer_cast<Flora::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Flora::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Flora::Timestep ts) override {
		// UPDATE
		m_CameraController.OnUpdate(ts);

		// RENDERING
		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();

		Flora::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto textureShader = m_ShaderLibrary.Get("Texture");
		auto squareShader = m_ShaderLibrary.Get("Square");

		std::dynamic_pointer_cast<Flora::OpenGLShader>(squareShader)->Bind();
		std::dynamic_pointer_cast<Flora::OpenGLShader>(squareShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				glm::vec3 pos(j * 0.11f, i * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Flora::Renderer::Submit(squareShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Flora::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_AlphaTexture->Bind();
		Flora::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Flora::Renderer::EndScene();
	}

	void OnEvent(Flora::Event& e) override { 
		m_CameraController.OnEvent(e);
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	Flora::ShaderLibrary m_ShaderLibrary;
	Flora::Ref<Flora::VertexArray> m_SquareVA;
	Flora::Ref<Flora::Texture2D> m_Texture, m_AlphaTexture;

	Flora::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.8f, 0.4f };
};

class Sandbox : public Flora::Application {
public:
	Sandbox() { PushLayer(new ExampleLayer()); }
	~Sandbox() { }
};

Flora::Application* Flora::CreateApplication() {
	return new Sandbox();
}