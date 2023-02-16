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
		m_SquarePosition(0.0f) {
		m_VertexArray.reset(Flora::VertexArray::Create());

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
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indicies[3] = { 0, 1, 2 };
		Flora::Ref<Flora::IndexBuffer> indexBuffer;
		indexBuffer.reset(Flora::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

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

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main() {
				color = vec4(u_Color, 1.0);
			}
		)";

		std::string textureShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main() {
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main() {
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_Shader.reset(Flora::Shader::Create(vertexSrc, fragmentSrc));
		m_SquareSH.reset(Flora::Shader::Create(vertexSrc2, fragmentSrc2));
		m_TextureShader.reset(Flora::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = Flora::Texture2D::Create("assets/textures/test.png");
	
		std::dynamic_pointer_cast<Flora::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Flora::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Flora::Timestep ts) override {
		FL_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

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
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		}
		else if (Flora::Input::IsKeyPressed(FL_KEY_D)) {
			m_SquarePosition.x += m_SquareMoveSpeed * ts;
		}
		if (Flora::Input::IsKeyPressed(FL_KEY_S)) {
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;
		}
		else if (Flora::Input::IsKeyPressed(FL_KEY_W)) {
			m_SquarePosition.y += m_SquareMoveSpeed * ts;
		}

		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Flora::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Flora::OpenGLShader>(m_SquareSH)->Bind();
		std::dynamic_pointer_cast<Flora::OpenGLShader>(m_SquareSH)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				glm::vec3 pos(j * 0.11f, i * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Flora::Renderer::Submit(m_SquareSH, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Flora::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), m_SquarePosition);
		//Flora::Renderer::Submit(m_Shader, m_VertexArray, triangleTransform);

		Flora::Renderer::EndScene();
	}

	void OnEvent(Flora::Event& event) override {
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	Flora::Ref<Flora::Shader> m_Shader;
	Flora::Ref<Flora::VertexArray> m_VertexArray;

	Flora::Ref<Flora::VertexArray> m_SquareVA;
	Flora::Ref<Flora::Shader> m_SquareSH, m_TextureShader;

	Flora::Ref<Flora::Texture2D> m_Texture;

	Flora::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotation = 0.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Flora::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};

Flora::Application* Flora::CreateApplication() {
	return new Sandbox();
}