#pragma once
#include "EditorPanel.h"
#include "Flora/Scene/Components.h"
#include <imgui/imgui.h>

namespace Flora {
	class PropertiesPanel : public EditorPanel {
	public:
		virtual void Initialize() override {};
		virtual void OnUpdate() override {};
		void OnImGuiRender() override;
	private:
		void DrawComponents(Entity entity);
		void DrawTextureDropbox(const std::string& label, SpriteRendererComponent& component, ImVec2 buttonSize);
	};
}