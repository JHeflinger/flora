#pragma once
#include "EditorPanel.h"
#include "Flora/Core/Base.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"

namespace Flora {
	class SceneHierarchyPanel : public EditorPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<EditorParams>& params);
		virtual void Initialize() override {};
		virtual void SetEditorContext(const Ref<EditorParams>& params) override;
		virtual void OnUpdate() override {};
		void OnImGuiRender() override;
		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Entity m_SelectionContext;
	};
}