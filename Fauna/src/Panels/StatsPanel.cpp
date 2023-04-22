#include "flpch.h"
#include "StatsPanel.h"
#include "Flora/Renderer/Renderer2D.h"
#include <imgui/imgui.h>

namespace Flora {
	void StatsPanel::OnImGuiRender() {
		auto rendererStats = Renderer2D::GetStats();
		ImGui::Begin("Stats", &m_Enabled);
		ImGui::Text("Draw Calls: %d", rendererStats.DrawCalls);
		ImGui::Text("Quads: %d", rendererStats.QuadCount);
		ImGui::Text("Vertices: %d", rendererStats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", rendererStats.GetTotalIndexCount());
		ImGui::Text("Frametime: %f", m_Frametime);

		int fps = (int)(1.0f / m_Frametime);
		if (fps < m_LowestFPS) m_LowestFPS = fps;
		if (fps > m_HighestFPS) m_HighestFPS = fps;
		ImGui::Text("FPS: %d", fps);
		ImGui::Text("Highest recorded FPS: %d", m_HighestFPS);
		ImGui::Text("Lowest recorded FPS: %d", m_LowestFPS);

		if (ImGui::Button("Reset Stats")) ResetStats();

		ImGui::End();
	}
}