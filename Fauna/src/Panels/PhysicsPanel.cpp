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
		ImGui::DragInt("##velit", m_EditorContext->ActiveScene->GetVelocityIterationRef(), 1, 0, 999999);
		ImGui::DragInt("##posin", m_EditorContext->ActiveScene->GetPositionIterationRef(), 1, 0, 999999);
		ImGui::DragFloat("##gravity", m_EditorContext->ActiveScene->GetGravityRef(), 0.1, 0, 999999, "%.2f");
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		ImGui::End();
	}
}