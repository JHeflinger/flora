#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include "Flora/Scene/Components.h"
#include "Flora/Utils/PlatformUtils.h"

namespace Flora {
	extern const std::filesystem::path g_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity) {
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.each([&](auto entityID) {
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};
		
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Create Empty Entity")) 
				m_Context->CreateEntity();
			ImGui::EndPopup();
		}
		
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext) 
			DrawComponents(m_SelectionContext);
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {

		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) 
			m_SelectionContext = entity;

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (entityDeleted) {
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity) {
				m_SelectionContext = {};
			}
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.6f, 0.0f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	static void DrawTextureDropbox(const std::string& label, SpriteRendererComponent& component, ImVec2 buttonSize) {
		if (ImGui::Button(label.c_str(), buttonSize)) {
			std::string filepath = FileDialogs::OpenFile("Texture Asset (*.png)\0*.png\0");
			if (!filepath.empty()) {
				std::filesystem::path texturePath = std::filesystem::path(filepath); // warning this is not relative
				component.Filename = texturePath.filename().string();
				component.Path = texturePath.string();
				component.Texture = Texture2D::Create(texturePath.string());
			}
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
				component.Filename = texturePath.filename().string();
				component.Path = texturePath.string();
				component.Texture = Texture2D::Create(texturePath.string());
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		ImGui::Text(component.Filename.c_str());
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uifunction, bool removeable = true) {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>()) {
			auto& component = entity.GetComponent<T>();
			ImVec2 contentReigionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			bool removeComponent = false;

			if (removeable) {
				ImGui::SameLine(contentReigionAvailable.x - lineHeight * 0.5f);

				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
					ImGui::OpenPopup("ComponentSettings");

				if (ImGui::BeginPopup("ComponentSettings")) {
					if (ImGui::MenuItem("Remove component"))
						removeComponent = true;
					ImGui::EndPopup();
				}
			}

			if (open) {
				auto& src = entity.GetComponent<T>();
				ImGui::Dummy(ImVec2(0, 5.0f));
				uifunction(component);
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity) {	
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = std::string(buffer);
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Component");
		if (ImGui::BeginPopup("Add Component")) {
			if (ImGui::MenuItem("Camera")) {
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer")) {
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale);
		}, false);

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Projection Type");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Primary Camera");
			ImGui::NextColumn();
			if (ImGui::BeginCombo("##Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Checkbox("##Primary", &component.Primary);
			ImGui::Columns(1);

			ImGui::Dummy(ImVec2(0, 10));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0, 10));

			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150.0f);
				ImGui::Text("Vertical FOV");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Near Clip");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Far Clip");
				ImGui::NextColumn();
				float perspectiveFOV = glm::degrees(component.Camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("##VerticalFOV", &perspectiveFOV))
					component.Camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));
				float perspectiveNear = component.Camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("##Near", &perspectiveNear))
					component.Camera.SetPerspectiveNearClip(perspectiveNear);
				float perspectiveFar = component.Camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("##Far", &perspectiveFar))
					component.Camera.SetPerspectiveFarClip(perspectiveFar);
				ImGui::Columns(1);
			}

			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150.0f);
				ImGui::Text("Orthographic Size");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Near Clip");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Far Clip");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Fixed Aspect Ratio");
				ImGui::NextColumn();
				float orthoSize = component.Camera.GetOrthographicSize();
				if (ImGui::DragFloat("##Size", &orthoSize))
					component.Camera.SetOrthographicSize(orthoSize);
				float orthoNear = component.Camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("##Near", &orthoNear))
					component.Camera.SetOrthographicNearClip(orthoNear);
				float orthoFar = component.Camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("##Far", &orthoFar))
					component.Camera.SetOrthographicFarClip(orthoFar);
				ImGui::Checkbox("##FixedAspectRatio", &component.FixedAspectRatio);
				ImGui::Columns(1);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite", entity, [](auto& component) {
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];
			const char* spriteTypeStrings[] = { "Single Texture", "Subtexture", "Animation" };
			const char* currentSpriteTypeString = spriteTypeStrings[(int)component.Type];
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 smallButtonSize = { lineHeight + 3.0f, lineHeight };
			ImVec2 largeButtonSize = { 100, lineHeight };
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
			ImGuiStyle& style = ImGui::GetStyle();
			style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add spacing between items in the column
			style.ColumnsMinSpacing = 20.0f;

			//==============================================Sprite type

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Type");
			ImGui::NextColumn();
			if (ImGui::BeginCombo("##Type", currentSpriteTypeString)) {
				for (int i = 0; i < 3; i++) {
					bool isSelected = currentSpriteTypeString == spriteTypeStrings[i];
					if (ImGui::Selectable(spriteTypeStrings[i], isSelected)) {
						currentSpriteTypeString = spriteTypeStrings[i];
						component.Type = (SpriteRendererComponent::SpriteType)i;
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Columns(1);

			ImGui::Dummy(ImVec2(0, 10.0f));
			ImGui::Separator(); //===========================separator
			ImGui::Dummy(ImVec2(0, 10.0f));

			if (component.Type == SpriteRendererComponent::SpriteType::SINGLE) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Texture");
				ImGui::NextColumn();
				DrawTextureDropbox("Texture", component, largeButtonSize);
				ImGui::Columns(1);

				ImGui::Dummy(ImVec2(0, 10.0f));
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
				bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Advanced Settings");
				if (open) {
					ImGui::Dummy(ImVec2(0, 5.0f));
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Dummy(ImVec2(0, 8.0f));
					ImGui::Text("Tint");
					ImGui::Dummy(ImVec2(0, 10.0f));
					ImGui::Text("Tiling Factor");
					ImGui::NextColumn();
					ImGui::Dummy(ImVec2(0, 5.0f));
					ImGui::ColorEdit4("##Tint", glm::value_ptr(component.Color));
					ImGui::Dummy(ImVec2(0, 5.0f));
					ImGui::DragFloat("##Tiling Factor", &component.TilingFactor, 0.1, 0.0f, 100.0f, "%.2f");
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}

			if (component.Type == SpriteRendererComponent::SpriteType::SUBTEXTURE) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Texture");
				ImGui::Dummy(ImVec2(0, 15.0f));
				ImGui::Text("Rows");
				ImGui::Dummy(ImVec2(0, 2.0f));
				ImGui::Text("Columns");
				ImGui::Dummy(ImVec2(0, 2.0f));
				ImGui::Text("Coordinate");
				ImGui::NextColumn();
				ImGui::PushItemWidth(60 + smallButtonSize.x * 2);
				DrawTextureDropbox("Subtexture", component, largeButtonSize);
				ImGui::Dummy(ImVec2(0, 5.0f));
				// rows, columns, subtexture row/col
				ImGui::DragInt("##Rows", &component.Rows, 0.1, 1, 1000);
				ImGui::DragInt("##Columns", &component.Columns, 0.1, 1, 1000);

				ImGui::PushItemWidth(30);
				style.ItemSpacing = ImVec2(0, 0); // remove spacing

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
				ImGui::PushFont(boldFont);
				if (ImGui::Button("R", smallButtonSize))
					component.RowCoordinate = 0;
				ImGui::PopStyleColor(3);
				ImGui::PopFont();

				ImGui::SameLine();
				ImGui::DragInt("##R", &component.RowCoordinate, 0.1f, 1, 10000);
				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
				ImGui::PushFont(boldFont);
				if (ImGui::Button("C", smallButtonSize))
					component.ColumnCoordinate = 0;
				ImGui::PopStyleColor(3);
				ImGui::PopFont();

				ImGui::SameLine();
				ImGui::DragInt("##C", &component.ColumnCoordinate, 0.1f, 1, 10000);

				ImGui::PopItemWidth();
				style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add back spacing

				ImGui::PopItemWidth();
				ImGui::Columns(1);

				ImGui::Dummy(ImVec2(0, 10.0f));
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
				bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Advanced Settings");
				if (open) {
					// subtexture width/height
					float linespacing = 2.0f;
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Width (Cells)");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Height (Cells)");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Tint");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Tiling Factor");
					ImGui::NextColumn();
					ImGui::DragFloat("##Width", &component.SubtextureWidth, 0.1, 0, 10000.0f);
					ImGui::DragFloat("##Height", &component.SubtextureHeight, 0.1, 0, 10000.0f);
					ImGui::ColorEdit4("##Tint", glm::value_ptr(component.Color));
					ImGui::DragFloat("##Tiling Factor", &component.TilingFactor, 0.1, 0.0f, 100.0f, "%.2f");
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}

			if (component.Type == SpriteRendererComponent::SpriteType::ANIMATION) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Texture");
				ImGui::Dummy(ImVec2(0, 15.0f));
				ImGui::Text("Rows");
				ImGui::Dummy(ImVec2(0, 2.0f));
				ImGui::Text("Columns");
				ImGui::NextColumn();
				ImGui::PushItemWidth(60 + smallButtonSize.x * 2);
				DrawTextureDropbox("Sprite Sheet", component, largeButtonSize);
				ImGui::Dummy(ImVec2(0, 5.0f));
				// rows, columns, subtexture row/col
				ImGui::DragInt("##Rows", &component.Rows, 0.1, 1, 1000);
				ImGui::DragInt("##Columns", &component.Columns, 0.1, 1, 1000);
				ImGui::PopItemWidth();
				ImGui::Columns(1);

				ImGui::Dummy(ImVec2(0, 10.0f));
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
				bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Advanced Settings");
				if (open) {
					// subtexture width/height
					float linespacing = 2.0f;
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Frames");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Start Frame");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("End Frame");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("FPS");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Tint");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Tiling Factor");
					ImGui::NextColumn();
					ImGui::DragInt("##Frames", &component.Frames, 0.1, 1, 1 + (component.Rows * component.Columns));
					ImGui::DragInt("##StartFrame", &component.StartFrame, 0.1, 1, 1 + component.Frames);
					ImGui::DragInt("##EndFrame", &component.EndFrame, 0.1, 1, 1 + component.Frames);
					ImGui::DragInt("##FPS", &component.FPS, 0.1, 1, 60);
					ImGui::ColorEdit4("##Tint", glm::value_ptr(component.Color));
					ImGui::DragFloat("##TilingFactor", &component.TilingFactor, 0.1, 0.0f, 10000.0f, "%.2f");
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}

			ImGui::PopStyleVar();
		});
	}
}