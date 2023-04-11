#include "flpch.h"
#include "StatsPanel.h"
#include "Flora/Renderer/Renderer2D.h"
#include <imgui/imgui.h>

namespace Flora {
	void StatsPanel::OnImGuiRender() {
		auto rendererStats = Renderer2D::GetStats();
		ImGui::Begin("Stats");
		ImGui::Text("Draw Calls: %d", rendererStats.DrawCalls);
		ImGui::Text("Quads: %d", rendererStats.QuadCount);
		ImGui::Text("Vertices: %d", rendererStats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", rendererStats.GetTotalIndexCount());
		ImGui::End();
	}
}