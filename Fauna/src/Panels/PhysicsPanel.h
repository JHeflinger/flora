#pragma once
#include "EditorPanel.h"

namespace Flora {
	class PhysicsPanel : public EditorPanel {
	public:
		PhysicsPanel() = default;
		virtual void Initialize() override {};
		virtual void OnUpdate() override {};
		void OnImGuiRender() override;
	};
}