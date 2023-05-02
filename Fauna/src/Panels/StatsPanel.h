#pragma once
#include "EditorPanel.h"
#include "Flora/Renderer/Renderer2D.h"

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
		float GetYMax();
	private:
		float m_Frametime = 0.0f;
		int m_LowestFPS = 99999;
		int m_HighestFPS = 0;
		int m_FrameCount = 0;
		Renderer2D::Statistics m_RendererStats;
	private:
		float m_TimeFrame = 5.0f; // frame of data recording in seconds
		bool m_ShowKey = true;
		bool m_ShowGraph = true;
		bool m_ShowDrawCalls = false;
		bool m_ShowQuads = false;
		bool m_ShowVertices = false;
		bool m_ShowIndices = false;
		bool m_ShowFrametime = true;
		bool m_ShowFPS = false;
		bool m_ShowLowestFPS = false;
		bool m_ShowHighestFPS = false;
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