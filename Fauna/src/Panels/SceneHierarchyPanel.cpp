#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Flora/Scene/Components.h"

namespace Flora {
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.each([&](auto entityID) {
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {

		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) 
			m_SelectionContext = entity;

		if (opened)
			ImGui::TreePop();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity) {
		if (entity.HasComponent<TagComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Tag")) {
				auto& tag = entity.GetComponent<TagComponent>().Tag;
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				if (ImGui::InputText("", buffer, sizeof(buffer)))
					tag = std::string(buffer);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "TransformComponent")) {
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {

				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)cameraComponent.Camera.GetProjectionType()];

				if (ImGui::Checkbox("Primary", &cameraComponent.Primary)) {
					//temporary inefficient but standalone solution so only one primary camera exists
					if (cameraComponent.Primary) {
						m_Context->m_Registry.each([&](auto entityID) {
							Entity entity{ entityID, m_Context.get() };
							if (entity.HasComponent<CameraComponent>()) {
								entity.GetComponent<CameraComponent>().Primary = false;
							}
						});
						cameraComponent.Primary = true;
					}
				}

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
					for (int i = 0; i < 2; i++) {
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStrings[i];
							cameraComponent.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (cameraComponent.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
					float perspectiveFOV = glm::degrees(cameraComponent.Camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveFOV))
						cameraComponent.Camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));
					float perspectiveNear = cameraComponent.Camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						cameraComponent.Camera.SetPerspectiveNearClip(perspectiveNear);
					float perspectiveFar = cameraComponent.Camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						cameraComponent.Camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (cameraComponent.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					float orthoSize = cameraComponent.Camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						cameraComponent.Camera.SetOrthographicSize(orthoSize);
					float orthoNear = cameraComponent.Camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						cameraComponent.Camera.SetOrthographicNearClip(orthoNear);
					float orthoFar = cameraComponent.Camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						cameraComponent.Camera.SetOrthographicFarClip(orthoFar);
					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}
	}
}