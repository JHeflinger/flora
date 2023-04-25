#include "flpch.h"
#include "StatsPanel.h"
#include "Flora/Renderer/Renderer2D.h"
#include <imgui/imgui.h>

namespace Flora {
	void StatsPanel::OnImGuiRender() {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;
		ImVec2 buttonSize = { 200, 0 };

		ImGui::Begin("Stats", &m_Enabled);
		if (ImGui::IsWindowHovered()) m_EditorContext->HoveredPanel = Panels::STATS;
		if (ImGui::IsWindowFocused()) m_EditorContext->FocusedPanel = Panels::STATS;

		if (ImGui::TreeNodeEx("Renderer Volume", treeNodeFlags)) {
			auto rendererStats = Renderer2D::GetStats();
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100);
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
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Video Performance", treeNodeFlags)) {
			int fps = (int)(1.0f / m_Frametime);
			if (fps < m_LowestFPS) m_LowestFPS = fps;
			if (fps > m_HighestFPS) m_HighestFPS = fps;
			float timestep = std::round(m_Frametime * 100000) / 100;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100);
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
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		ImGui::End();
	}
}