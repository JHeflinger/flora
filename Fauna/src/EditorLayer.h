#pragma once
#include "Flora.h"

namespace Flora {
	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttatch() override;
		virtual void OnDetatch() override;
		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		OrthographicCameraController m_CameraController;

		//temp 
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_WizardTexture;
		Ref<Framebuffer> m_Framebuffer;

		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
	};
}