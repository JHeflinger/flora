#include "flpch.h"
#include "PhysicsPanel.h"
#include <imgui/imgui.h>

namespace Flora {
	void PhysicsPanel::OnImGuiRender() {
		ImGui::Begin("Physics");
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 170.0f);
		ImGui::Dummy({ 0.0f, 2.0f });
		ImGui::Text("Velocity Iterations");
		ImGui::Dummy({ 0.0f, 2.0f });
		ImGui::Text("Position Iterations");
		ImGui::Dummy({ 0.0f, 2.0f });
		ImGui::Text("Gravity");
		ImGui::Dummy({ 0.0f, 2.0f });
		ImGui::Text("Visible Colliders");
		ImGui::Dummy({ 0.0f, 2.0f });
		ImGui::NextColumn();
		ImGui::PushItemWidth(100);
		static int velin = 0;
		static int posin = 0;
		static float gravity = 9.8f;
		ImGui::DragInt("##velit", m_EditorContext->ActiveScene->GetVelocityIterationRef(), 1.0f, 0, 999999);
		ImGui::DragInt("##posin", m_EditorContext->ActiveScene->GetPositionIterationRef(), 1.0f, 0, 999999);
		ImGui::DragFloat("##gravity", m_EditorContext->ActiveScene->GetGravityRef(), 0.1f, 0, 999999, "%.2f");
		ImGui::Dummy({ 0.0f, 1.0f });
		ImGui::Checkbox("##visiblecolliders", &(m_EditorContext->VisibleColliders));
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		ImGui::End();
	}
}