#pragma once
#include "EditorPanel.h"
#include "Flora/Core/Base.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"

namespace Flora {
	class SceneHierarchyPanel : EditorPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);
		virtual void Initialize() override {};
		virtual void SetSceneContext(const Ref<Scene>& context) override;
		virtual void OnUpdate() override {};
		void OnImGuiRender() override;
		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}