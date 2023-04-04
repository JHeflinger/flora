#pragma once
#include "Flora.h"
#include "Panels/SceneHierarchyPanel.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);
		void SaveSceneAs();
		void OpenScene();
		void NewScene();
	private:
		// Camera controller
		OrthographicCameraController m_CameraController;

		// Viewport framebuffer
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0, 0 };
		bool m_ViewportFocused, m_ViewportHovered = false;

		// Scene
		Ref<Scene> m_ActiveScene;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

		// Gizmos
		int m_GizmoType = -1;
	};
}