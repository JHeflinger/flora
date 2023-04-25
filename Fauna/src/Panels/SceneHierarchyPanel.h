#pragma once
#include "EditorPanel.h"
#include "Flora/Scene/Entity.h"

namespace Flora {
	class SceneHierarchyPanel : public EditorPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<EditorParams>& params);
		virtual void Initialize() override {};
		virtual void OnUpdate() override {};
		void OnImGuiRender() override;
		void DeleteEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
	};
}