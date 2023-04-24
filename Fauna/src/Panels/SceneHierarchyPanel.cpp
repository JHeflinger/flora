#include "SceneHierarchyPanel.h"
#include "Flora/Scene/Components.h"
#include <imgui/imgui.h>

namespace Flora {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<EditorParams>& params) {
		SetEditorContext(params);
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene Hierarchy", &m_Enabled);
		if (ImGui::IsWindowHovered()) m_EditorContext->HoveredPanel = Panels::SCENEHIERARCHY;
		if (ImGui::IsWindowFocused()) m_EditorContext->FocusedPanel = Panels::SCENEHIERARCHY;
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
		if (!m_EditorContext->ActiveScene->EntityExists(entity)) return;
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_EditorContext->SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) 
			m_EditorContext->SelectedEntity = entity;
		if (ImGui::BeginDragDropSource()) {
			uint32_t entityHandle = (uint32_t)entity;
			ImGui::SetDragDropPayload("HIERARCHY_DRAG_ENTITY", &entityHandle, sizeof(&entityHandle));
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAG_ENTITY")) {
				Entity draggedEntity{ (entt::entity)(*(const uint32_t*)payload->Data), m_EditorContext->ActiveScene.get() };
				if (draggedEntity.HasComponent<ParentComponent>())
					draggedEntity.GetComponent<ParentComponent>().Parent.GetComponent<ChildComponent>().RemoveChild(draggedEntity);
				if (!entity.HasComponent<ChildComponent>()) entity.AddComponent<ChildComponent>();
				if (!draggedEntity.HasComponent<ParentComponent>()) draggedEntity.AddComponent<ParentComponent>();
				draggedEntity.GetComponent<ParentComponent>().Parent = entity;
				entity.GetComponent<ChildComponent>().AddChild(draggedEntity);
			}
			ImGui::EndDragDropTarget();
		}

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
			if (entity.HasComponent<ParentComponent>()) {
				if (ImGui::MenuItem("Bring Entity Out")) {
					entity.GetComponent<ParentComponent>().Parent.GetComponent<ChildComponent>().RemoveChild(entity);
					entity.RemoveComponent<ParentComponent>();
				}
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
			DeleteEntity(entity);
			if (m_EditorContext->SelectedEntity == entity) {
				m_EditorContext->SelectedEntity = {};
			}
		}
	}

	void SceneHierarchyPanel::DeleteEntity(Entity entity) {
		if (entity.HasComponent<ChildComponent>()) {
			std::vector<Entity> children = entity.GetComponent<ChildComponent>().Children;
			for (int i = 0; i < children.size(); i++) {
				DeleteEntity(children[i]);
			}
		}
		if (entity.HasComponent<ParentComponent>()) 
			entity.GetComponent<ParentComponent>().Parent.GetComponent<ChildComponent>().RemoveChild(entity);
		m_EditorContext->ActiveScene->DestroyEntity(entity);
	}
}