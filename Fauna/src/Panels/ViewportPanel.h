#pragma once
#include "EditorPanel.h"
#include "Flora/Renderer/Framebuffer.h"
#include "Flora/Scene/Entity.h"

namespace Flora {
	class ViewportPanel : public EditorPanel {
	public:
		virtual void Initialize() override;
		virtual void OnUpdate() override;
		void PreUpdate();
		void PostUpdate();
		virtual void OnImGuiRender() override;
		glm::vec2 GetViewportSize() const { return m_ViewportSize; }
		bool ViewportResized();
		Entity GetHoveredEntity() const { return m_HoveredEntity; }
	public:
		void RequestOpenScene(std::string filepath) { m_OpenNewScene = true; newScenePath = filepath; }
		bool IsRequestingOpenScene() { return m_OpenNewScene; }
		void ResolveOpenSceneRequest() { m_OpenNewScene = false; }
		std::string GetRequestedStringPath() { return newScenePath; }
	private:
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0, 0 };
		glm::vec2 m_ViewportBounds[2];
		Entity m_HoveredEntity;
	private:
		bool m_OpenNewScene = false;
		std::string newScenePath = "";
	};
}