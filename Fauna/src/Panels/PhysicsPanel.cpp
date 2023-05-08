#include "flpch.h"
#include "PhysicsPanel.h"
#include <imgui/imgui.h>

namespace Flora {
	void PhysicsPanel::OnImGuiRender() {
		ImGui::Begin("Physics");
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 170);
		ImGui::Dummy({ 0, 2 });
		ImGui::Text("Velocity Iterations");
		ImGui::Dummy({ 0, 2 });
		ImGui::Text("Position Iterations");
		ImGui::Dummy({ 0, 2 });
		ImGui::Text("Gravity");
		ImGui::Dummy({ 0, 2 });
		ImGui::NextColumn();
		ImGui::PushItemWidth(100);
		static int velin = 0;
		static int posin = 0;
		static float gravity = 9.8f;
		ImGui::DragInt("##velit", &velin, 0.1, 0, 999999, "%.2f");
		ImGui::DragInt("##posin", &posin, 0.1, 0, 999999, "%.2f");
		ImGui::DragFloat("##gravity", &gravity, 0.1, 0, 999999, "%.2f");
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		ImGui::End();
	}
}