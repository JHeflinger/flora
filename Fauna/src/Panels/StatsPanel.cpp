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
			auto rendererStats = Renderer2D::GetStats();
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
			ImGui::Button(std::to_string(rendererStats.DrawCalls).c_str(), buttonSize);
			ImGui::Button(std::to_string(rendererStats.QuadCount).c_str(), buttonSize);
			ImGui::Button(std::to_string(rendererStats.GetTotalVertexCount()).c_str(), buttonSize);
			ImGui::Button(std::to_string(rendererStats.GetTotalIndexCount()).c_str(), buttonSize);
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

	void StatsPanel::OnUpdate() {
		m_FrameCount++;
		auto rendererStats = Renderer2D::GetStats();
		int fps = (int)(1.0f / m_Frametime);
		m_FrameCountData.emplace_back((float)m_FrameCount);
		m_FrametimeData.emplace_back(1000 * m_Frametime);
		m_DrawCallsData.emplace_back(rendererStats.DrawCalls);
		m_QuadsData.emplace_back(rendererStats.QuadCount);
		m_VerticesData.emplace_back(rendererStats.GetTotalVertexCount());
		m_IndicesData.emplace_back(rendererStats.GetTotalIndexCount());
		m_FPSData.emplace_back(fps);
		m_LowestFPSData.emplace_back(m_LowestFPS);
		m_HighestFPSData.emplace_back(m_HighestFPS);
	}
}