#include <Flora.h>
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Flora::Layer {
public:
	ExampleLayer() 
		: Layer("Example"), 
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_CameraPosition(0.0f),
		m_TrianglePosition(0.0f) {
		m_TriangleVA.reset(Flora::VertexArray::Create());

		float verticies[3 * 7] = {
			-0.5f, -0.5f,  0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Flora::Ref<Flora::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Flora::VertexBuffer::Create(verticies, sizeof(verticies)));
		Flora::BufferLayout layout = {
			{ Flora::ShaderDataType::Float3, "a_Position" },
			{ Flora::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(vertexBuffer);

		uint32_t indicies[3] = { 0, 1, 2 };
		Flora::Ref<Flora::IndexBuffer> indexBuffer;
		indexBuffer.reset(Flora::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(indexBuffer);

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
		auto triangleShader = m_ShaderLibrary.Load("assets/shaders/Triangle.glsl");
		auto squareShader = m_ShaderLibrary.Load("assets/shaders/Square.glsl");

		m_Texture = Flora::Texture2D::Create("assets/textures/test.png");
		m_AlphaTexture = Flora::Texture2D::Create("assets/textures/testalpha.png");
	
		std::dynamic_pointer_cast<Flora::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Flora::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Flora::Timestep ts) override {

		if (Flora::Input::IsKeyPressed(FL_KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}else if (Flora::Input::IsKeyPressed(FL_KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		if (Flora::Input::IsKeyPressed(FL_KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}else if (Flora::Input::IsKeyPressed(FL_KEY_UP)) {
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}

		if (Flora::Input::IsKeyPressed(FL_KEY_Q)) {
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}else if (Flora::Input::IsKeyPressed(FL_KEY_E)) {
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		if (Flora::Input::IsKeyPressed(FL_KEY_A)) {
			m_TrianglePosition.x -= m_TriangleMoveSpeed * ts;
		}
		else if (Flora::Input::IsKeyPressed(FL_KEY_D)) {
			m_TrianglePosition.x += m_TriangleMoveSpeed * ts;
		}
		if (Flora::Input::IsKeyPressed(FL_KEY_S)) {
			m_TrianglePosition.y -= m_TriangleMoveSpeed * ts;
		}
		else if (Flora::Input::IsKeyPressed(FL_KEY_W)) {
			m_TrianglePosition.y += m_TriangleMoveSpeed * ts;
		}

		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Flora::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto textureShader = m_ShaderLibrary.Get("Texture");
		auto triangleShader = m_ShaderLibrary.Get("Triangle");
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

		glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), m_TrianglePosition);
		Flora::Renderer::Submit(triangleShader, m_TriangleVA, triangleTransform);

		Flora::Renderer::EndScene();
	}

	void OnEvent(Flora::Event& event) override { }

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	Flora::ShaderLibrary m_ShaderLibrary;
	Flora::Ref<Flora::VertexArray> m_TriangleVA, m_SquareVA;
	Flora::Ref<Flora::Texture2D> m_Texture, m_AlphaTexture;

	Flora::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotation = 0.0f;

	glm::vec3 m_TrianglePosition;
	float m_TriangleMoveSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Flora::Application {
public:
	Sandbox() { PushLayer(new ExampleLayer()); }
	~Sandbox() { }
};

Flora::Application* Flora::CreateApplication() {
	return new Sandbox();
}