#pragma once
#include "Flora.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/StatsPanel.h"
#include "Flora/Renderer/EditorCamera.h"
#include "Flora/Utils/Structures.h"

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
		void SaveScene();
		void OpenScene(const std::filesystem::path& path);
		void NewScene();
		void OnOverrideEvent();
	private:
		// Viewport
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0, 0 };
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused, m_ViewportHovered = false;
		Entity m_HoveredEntity;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		StatsPanel m_StatsPanel;

		// OverrideEvent - temp solution
		bool m_OverrideEventReady = true;

		// Serializer Settings
		EditorParams m_EditorParams;
	};
}