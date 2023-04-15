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
		bool ViewportHovered() const { return m_ViewportHovered; }
		bool ViewportFocused() const { return m_ViewportFocused; }
		Entity GetHoveredEntity() const { return m_HoveredEntity; }
	private:
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0, 0 };
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused, m_ViewportHovered = false;
		Entity m_HoveredEntity;
	};
}