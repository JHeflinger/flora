#pragma once
#include "EditorPanel.h"

namespace Flora {
	class StatsPanel : public EditorPanel {
	public:
		StatsPanel() = default;
		virtual void Initialize() override {};
		virtual void OnUpdate() override {};
		virtual void OnImGuiRender() override;
	private:
	};
}