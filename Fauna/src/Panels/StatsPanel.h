#pragma once
#include "EditorPanel.h"

namespace Flora {
	class StatsPanel : public EditorPanel {
	public:
		StatsPanel() = default;
		virtual void Initialize() override {};
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		void UpdateFrametime(float ts) { m_Frametime = ts; }
	private:
		void ResetStats() {
			m_Frametime = 0.0f;
			m_LowestFPS = 99999;
			m_HighestFPS = 0;
			m_FrameCount = 0;
			m_DrawCallsData.clear();
			m_QuadsData.clear();
			m_VerticesData.clear();
			m_IndicesData.clear();
			m_FrametimeData.clear();
			m_FrameCountData.clear();
			m_FPSData.clear();
			m_LowestFPSData.clear();
			m_HighestFPSData.clear();
		}
	private:
		float m_Frametime = 0.0f;
		int m_LowestFPS = 99999;
		int m_HighestFPS = 0;
		int m_FrameCount = 0;
	private:
		bool m_ShowGraph = true;
		bool m_ShowDrawCalls = true;
		bool m_ShowQuads = true;
		bool m_ShowVertices = true;
		bool m_ShowIndices = true;
		bool m_ShowFrametime = true;
		bool m_ShowFPS = true;
		bool m_ShowLowestFPS = true;
		bool m_ShowHighestFPS = true;
		std::vector<float> m_DrawCallsData;
		std::vector<float> m_QuadsData;
		std::vector<float> m_VerticesData;
		std::vector<float> m_IndicesData;
		std::vector<float> m_FrametimeData;
		std::vector<float> m_FrameCountData;
		std::vector<float> m_FPSData;
		std::vector<float> m_LowestFPSData;
		std::vector<float> m_HighestFPSData;
	};
}