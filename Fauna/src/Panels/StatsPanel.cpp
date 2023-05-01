#include "flpch.h"
#include "StatsPanel.h"
#include "Flora/Renderer/Renderer2D.h"
#include <imgui/imgui.h>
#include <imgui/implot.h>

namespace Flora {
	void StatsPanel::OnImGuiRender() {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;
		ImVec2 buttonSize = { 180, 0 };

		ImGui::Begin("Stats", &m_Enabled);
		if (ImGui::IsWindowHovered()) m_EditorContext->HoveredPanel = Panels::STATS;
		if (ImGui::IsWindowFocused()) m_EditorContext->FocusedPanel = Panels::STATS;

		if (ImGui::TreeNodeEx("Renderer Volume", treeNodeFlags)) {
			m_RendererStats = Renderer2D::GetStats();
			ImGui::Columns(3);
			ImGui::SetColumnWidth(0, 100);
			ImGui::SetColumnWidth(1, 200);
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Draw Calls");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Quads");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Vertices");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Indices");
			ImGui::Dummy({ 0, 2 });
			ImGui::NextColumn();
			ImGui::Button(std::to_string(m_RendererStats.DrawCalls).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_RendererStats.QuadCount).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_RendererStats.GetTotalVertexCount()).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_RendererStats.GetTotalIndexCount()).c_str(), buttonSize);
			ImGui::NextColumn();
			ImGui::Checkbox("##drawcalls", &m_ShowDrawCalls);
			ImGui::Checkbox("##quads", &m_ShowQuads);
			ImGui::Checkbox("##vertices", &m_ShowVertices);
			ImGui::Checkbox("##indices", &m_ShowIndices);
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Video Performance", treeNodeFlags)) {
			int fps = (int)(1.0f / m_Frametime);
			if (fps < m_LowestFPS) m_LowestFPS = fps;
			if (fps > m_HighestFPS) m_HighestFPS = fps;
			float timestep = std::round(m_Frametime * 100000) / 100;
			ImGui::Columns(3);
			ImGui::SetColumnWidth(0, 100);
			ImGui::SetColumnWidth(1, 200);
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Timestep");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("FPS");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("FPS Floor");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("FPS Ceiling");
			ImGui::Dummy({ 0, 2 });
			ImGui::NextColumn();
			ImGui::Button((std::to_string(timestep) + " us").c_str(), buttonSize);
			ImGui::Button(std::to_string(fps).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_LowestFPS).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_HighestFPS).c_str(), buttonSize);
			ImGui::NextColumn();
			ImGui::Checkbox("##frametime", &m_ShowFrametime);
			ImGui::Checkbox("##fps", &m_ShowFPS);
			ImGui::Checkbox("##fpsfloor", &m_ShowLowestFPS);
			ImGui::Checkbox("##fpsceiling", &m_ShowHighestFPS);
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		if (m_ShowGraph) {
			ImGui::Begin("Graphs");

			ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 0,0,0,0 });
			ImPlot::PushStyleColor(ImPlotCol_AxisBgHovered, { 0,0,0,0 });
			ImPlot::PushStyleColor(ImPlotCol_AxisBgActive, { 0,0,0,0 });

			ImPlot::SetNextAxesLimits(m_FrameCountData.front(), m_FrameCountData.back(), 0, GetYMax(), ImPlotCond_Always);

			ImPlotFlags flags = ImPlotFlags_NoTitle;
			if (ImPlot::BeginPlot("Graph data", { 0, 0 }, flags)) {
				float* frameCountData = &m_FrameCountData[0];

				if (m_ShowDrawCalls) {
					float* drawCallData = &m_DrawCallsData[0];
					ImPlot::PlotLine("Draw Calls", frameCountData, drawCallData, m_FrameCountData.size());
				}

				if (m_ShowQuads) {
					float* drawQuadData = &m_QuadsData[0];
					ImPlot::PlotLine("Quads", frameCountData, drawQuadData, m_FrameCountData.size());
				}

				if (m_ShowVertices) {
					float* vertexData = &m_VerticesData[0];
					ImPlot::PlotLine("Vertices", frameCountData, vertexData, m_FrameCountData.size());
				}

				if (m_ShowIndices) {
					float* indexData = &m_IndicesData[0];
					ImPlot::PlotLine("Indices", frameCountData, indexData, m_FrameCountData.size());
				}

				if (m_ShowFrametime) {
					float* frametimeData = &m_FrametimeData[0];
					ImPlot::PlotLine("Frame time (us)", frameCountData, frametimeData, m_FrameCountData.size());
				}

				if (m_ShowFPS) {
					float* fpsData = &m_FPSData[0];
					ImPlot::PlotLine("FPS", frameCountData, fpsData, m_FrameCountData.size());
				}

				if (m_ShowLowestFPS) {
					float* lowestFPSData = &m_LowestFPSData[0];
					ImPlot::PlotLine("FPS Floor", frameCountData, lowestFPSData, m_FrameCountData.size());
				}

				if (m_ShowHighestFPS) {
					float* highestFPSData = &m_HighestFPSData[0];
					ImPlot::PlotLine("FPS Ceiling", frameCountData, highestFPSData, m_FrameCountData.size());
				}

				ImPlot::EndPlot();
			}

			ImPlot::PopStyleColor(3);
			ImGui::End();
		}

		ImGui::End();
	}

	static void EmplaceAndPop(std::vector<float>& data, float newpoint, bool pop) {
		data.emplace_back(newpoint);
		if (pop) data.erase(data.begin());
	}

	void StatsPanel::OnUpdate() {
		m_FrameCount++;
		int fps = (1.0f / m_Frametime);

		// limit to time frame
		float period = 0.0f;
		for (int i = 0; i < m_FrametimeData.size(); i++)
			period += m_FrametimeData[i];
		bool pop = period > (m_TimeFrame * 1000.0f);

		EmplaceAndPop(m_FrameCountData, (float)m_FrameCount, pop);
		EmplaceAndPop(m_FrametimeData, 1000 * m_Frametime, pop);
		EmplaceAndPop(m_DrawCallsData, (float)(m_RendererStats.DrawCalls), pop);
		EmplaceAndPop(m_QuadsData, (float)(m_RendererStats.QuadCount), pop);
		EmplaceAndPop(m_VerticesData, (float)(m_RendererStats.GetTotalVertexCount()), pop);
		EmplaceAndPop(m_IndicesData, (float)(m_RendererStats.GetTotalIndexCount()), pop);
		EmplaceAndPop(m_FPSData, fps, pop);
		EmplaceAndPop(m_LowestFPSData, m_LowestFPS, pop);
		EmplaceAndPop(m_HighestFPSData, m_HighestFPS, pop);
	}

	static float GetMaxFromVector(std::vector<float> vec) {
		return (float)(*std::max_element(vec.begin(), vec.end()));
	}

	static float CascadeMax(float oldMax, float newMax) {
		return newMax > oldMax ? newMax : oldMax;
	}

	float StatsPanel::GetYMax() {
		float max = 0.0f;
		if (m_ShowFrametime) max = CascadeMax(max, 60.0f);
		if (m_ShowDrawCalls) max = CascadeMax(max, 10.0f);
		if (m_ShowQuads) max = CascadeMax(max, GetMaxFromVector(m_QuadsData) * 1.1f);
		if (m_ShowVertices) max = CascadeMax(max, GetMaxFromVector(m_VerticesData) * 1.1f);
		if (m_ShowIndices) max = CascadeMax(max, GetMaxFromVector(m_IndicesData) * 1.1f);
		if (m_ShowFPS) max = CascadeMax(max, 144);
		if (m_ShowLowestFPS) max = CascadeMax(max, GetMaxFromVector(m_LowestFPSData) * 1.1f);
		if (m_ShowHighestFPS) max = CascadeMax(max, GetMaxFromVector(m_HighestFPSData) * 1.1f);
		return max;
	}
}