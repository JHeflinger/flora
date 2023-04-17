#pragma once
#include "EditorPanel.h"

namespace Flora {
	class PropertiesPanel : public EditorPanel {
	public:
		virtual void Initialize() override {};
		virtual void OnUpdate() override {};
		void OnImGuiRender() override;
	private:
		void DrawComponents(Entity entity);
	};
}