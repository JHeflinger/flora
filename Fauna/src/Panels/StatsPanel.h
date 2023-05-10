#pragma once
#pragma comment(lib, "pdh.lib")
#include "EditorPanel.h"
#include "Flora/Renderer/Renderer2D.h"
#include <Pdh.h>

namespace Flora {
	class StatsPanel : public EditorPanel {
	public:
		StatsPanel() = default;
		virtual void Initialize() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		void UpdateFrametime(float ts) { m_Frametime = ts; }
	private:
		void ResetStats() {
			m_Frametime = 0.0f;
			m_LowestFPS = 99999;
			m_HighestFPS = 0;
			m_FrameCount = 0;
			for (auto& datavec : m_DataMap)
				datavec.second.clear();
			m_FrameCountData.clear();
		}
		float GetYMax();
	private:
		float m_Frametime = 0.0f;
		int m_LowestFPS = 99999;
		int m_HighestFPS = 0;
		int m_FrameCount = 0;
		Renderer2D::Statistics m_RendererStats;
	private:
		std::map<Stats, std::vector<float>> m_DataMap;
		std::vector<float> m_FrameCountData;
		bool m_ShowGraph = true;
	private:
		float m_HardwareCollectionDelay = 0.0f;
		double m_Ramsize = 0.0f; // in gigabytes
		PDH_HQUERY cpuQuery;
		PDH_HCOUNTER cpuTotal;
		PDH_FMT_COUNTERVALUE cpuCounterVal;
		PDH_HQUERY gpuQuery;
		PDH_HCOUNTER gpuTotal;
		PDH_FMT_COUNTERVALUE gpuCounterVal;
		PDH_HQUERY memQuery;
		PDH_HCOUNTER memTotal;
		PDH_FMT_COUNTERVALUE memCounterVal;
		PDH_HQUERY diskQuery;
		PDH_HCOUNTER diskTotal;
		PDH_FMT_COUNTERVALUE diskCounterVal;
	};
}