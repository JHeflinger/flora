#pragma once
#include "Flora.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Flora/Renderer/EditorCamera.h"

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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void SaveSceneAs();
		void OpenScene();
		void NewScene();
		void OnOverrideEvent();
	private:
		// Editor Camera
		EditorCamera m_EditorCamera;

		// Viewport
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0, 0 };
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused, m_ViewportHovered = false;
		Entity m_HoveredEntity;

		// Scene
		Ref<Scene> m_ActiveScene;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

		// Gizmos
		int m_GizmoType = -1;

		// OverrideEvent
		bool m_OverrideEventReady = true;
	};
}