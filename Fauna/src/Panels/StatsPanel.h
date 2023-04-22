#pragma once
#include "EditorPanel.h"

namespace Flora {
	class StatsPanel : public EditorPanel {
	public:
		StatsPanel() = default;
		virtual void Initialize() override {};
		virtual void OnUpdate() override {};
		virtual void OnImGuiRender() override;
		void UpdateFrametime(float ts) { m_Frametime = ts; }
	private:
		void ResetStats() {
			m_Frametime = 0.0f;
			m_LowestFPS = 9999999;
			m_HighestFPS = 0;
		}
	private:
		float m_Frametime = 0.0f;
		int m_LowestFPS = 9999999;
		int m_HighestFPS = 0;
	};
}