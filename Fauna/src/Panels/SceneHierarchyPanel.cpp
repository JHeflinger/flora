#include "SceneHierarchyPanel.h"
#include "Flora/Scene/Components.h"
#include <imgui/imgui.h>

namespace Flora {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<EditorParams>& params) {
		SetEditorContext(params);
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene Hierarchy", &m_Enabled);
		m_EditorContext->ActiveScene->m_Registry.each([&](auto entityID) {
			Entity entity{ entityID, m_EditorContext->ActiveScene.get() };
			if (!entity.HasComponent<ParentComponent>())
				DrawEntityNode(entity);
		});
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_EditorContext->SelectedEntity = {};
		
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Create New Entity")) 
				m_EditorContext->ActiveScene->CreateEntity();
			ImGui::EndPopup();
		}
		
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {

		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_EditorContext->SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) 
			m_EditorContext->SelectedEntity = entity;

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			if (ImGui::MenuItem("Create New Entity")) {
				Entity newChild = m_EditorContext->ActiveScene->CreateEntity();
				newChild.AddComponent<ParentComponent>().Parent = entity;
				if (!entity.HasComponent<ChildComponent>())
					entity.AddComponent<ChildComponent>().AddChild(newChild);
				else entity.GetComponent<ChildComponent>().AddChild(newChild);
			}
			ImGui::EndPopup();
		}

		if (opened) {
			if (entity.HasComponent<ChildComponent>()) {
				std::vector<Entity> children = entity.GetComponent<ChildComponent>().Children;
				for (int i = 0; i < children.size(); i++)
					DrawEntityNode(children[i]);
			}
			ImGui::TreePop();
		}

		if (entityDeleted) {
			m_EditorContext->ActiveScene->DestroyEntity(entity);
			if (m_EditorContext->SelectedEntity == entity) {
				m_EditorContext->SelectedEntity = {};
			}
		}
	}
}