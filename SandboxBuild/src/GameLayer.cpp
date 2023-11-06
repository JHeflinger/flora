#include "flpch.h"
#include "GameLayer.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Scripting/ScriptEngine.h"

namespace Flora {

	static Ref<Framebuffer> s_Framebuffer;

	GameLayer::GameLayer()
		: Layer("Game") {
		ScriptEngine::Init();
		m_ActiveScene = CreateRef<Scene>();
		FL_CORE_INFO(Serializer::DeserializeRawScene(m_ActiveScene, m_SceneManager.GetStartingScene()));
	}

	void GameLayer::OnAttatch() {
		RenderCommand::Init();
	}

	void GameLayer::OnDetatch() {
	}

	void GameLayer::OnUpdate(Timestep ts) {
		EditorCamera camera;
		camera.SetCameraTypeWithString("Orthographic");
		camera.SetOrthographicSize(10.0f);

		Renderer2D::ResetStats();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		Renderer2D::BeginScene(camera.GetViewProjection());
		Renderer2D::DrawLine({-50, -50, -50}, {50, 50, 50});
		Renderer2D::EndScene();
	}

	void GameLayer::OnImGuiRender() {

	}

	void GameLayer::OnEvent(Event& e) {

	}
}