#include "flpch.h"
#include "PropertiesPanel.h"
#include "Flora/Utils/PlatformUtils.h"
#include "Flora/Scripting/ScriptEngine.h"
#include <imgui/imgui_internal.h>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

// temp until stable alternative is implemented
#include "../../assets/scripts/MasterNativeScript.h"

namespace Flora {
	extern const std::filesystem::path g_AssetPath;

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
				uifunction(entity, component);
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	template<typename T>
	static void DrawAddComponentItem(std::string label, Entity entity) {
		if (!entity.HasComponent<T>()) {
			if (ImGui::MenuItem(label.c_str())) {
				entity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
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
				component.TextureInitialized = false;
			}
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
				if (texturePath.extension().string() == ".png") {
					component.Filename = texturePath.filename().string();
					component.Path = texturePath.string();
					component.TextureInitialized = false;
				} else {
					FL_CORE_ERROR("Invalid texture type. Please use a .png file");
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		ImGui::Text(component.Filename.c_str());
	}

	static void DrawScriptManagerDropbox(std::vector<NativeScriptComponent>& nscs, int newIndex) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::Button("##index", { ImGui::GetWindowWidth() - 50, 3 });
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_SCRIPT")) {
				int oldIndex = *(const int*)payload->Data;
				if (oldIndex < newIndex) newIndex--;
				FL_CORE_INFO(std::to_string(oldIndex));
				NativeScriptComponent element = nscs[oldIndex];
				nscs.erase(nscs.begin() + oldIndex);
				nscs.insert(nscs.begin() + newIndex, element);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopStyleColor(3);
	}

	void PropertiesPanel::OnImGuiRender() {
		ImGui::Begin("Properties", &m_Enabled);
		if (ImGui::IsWindowHovered()) m_EditorContext->HoveredPanel = Panels::PROPERTIES;
		if (ImGui::IsWindowFocused()) m_EditorContext->FocusedPanel = Panels::PROPERTIES;
		Entity selectedEntity = m_EditorContext->SelectedEntity;
		if (selectedEntity)
			DrawComponents(selectedEntity);
		ImGui::End();
	}

	void PropertiesPanel::DrawComponents(Entity entity) {
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
			DrawAddComponentItem<CameraComponent>("Camera", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<SpriteRendererComponent>("Sprite Renderer", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<CircleRendererComponent>("Circle Renderer", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<ScriptComponent>("Script", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<NativeScriptComponent>("Native Script", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<ScriptManagerComponent>("Script Manager", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<RigidBody2DComponent>("Rigid Body 2D", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<BoxCollider2DComponent>("Box Collider 2D", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<CircleCollider2DComponent>("Circle Collider 2D", m_EditorContext->SelectedEntity);
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		ImGuiStyle& style = ImGui::GetStyle();
		style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add spacing between items in the column
		style.ColumnsMinSpacing = 20.0f;

		DrawComponent<ParentComponent>("Parent", entity, [](auto& entity, auto& component) {
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Inherit All");
			ImGui::NextColumn();
			ImGui::Checkbox("##InheritAll", &component.InheritAll);
			ImGui::Columns(1);

			ImGui::Dummy(ImVec2(0, 10.0f));
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			bool open = ImGui::TreeNodeEx((void*)typeid(ParentComponent).hash_code(), treeNodeFlags, "Advanced Settings");
			if (open) {
				if (component.InheritAll)
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150.0f);
				ImGui::Dummy(ImVec2(0, 8.0f));
				ImGui::Text("Inherit Transform");
				ImGui::NextColumn();
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Checkbox("##InheritTransform", &component.InheritTransform);
				ImGui::Columns(1);
				ImGui::TreePop();
				if (component.InheritAll)
					ImGui::PopStyleVar();
			}
		}, false);

		DrawComponent<TransformComponent>("Transform", entity, [](auto& entity, auto& component) {
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale);
		}, false);

		DrawComponent<CameraComponent>("Camera", entity, [](auto& entity, auto& component) {
			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Projection Type");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Primary Camera");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Visible Borders");
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

			int primaryCameraEntity = entity.GetScene()->GetPrimaryCamera();
			bool isPrimary = false;
			if (primaryCameraEntity >= 0)
				isPrimary = (uint32_t)(primaryCameraEntity) == (uint32_t)entity;
			if (ImGui::Checkbox("##Primary", &isPrimary)) {
				if (isPrimary) {
					entity.GetScene()->SetPrimaryCamera((int)(uint32_t)entity);
				}
				else {
					entity.GetScene()->SetPrimaryCamera(-1);
				}
			}

			ImGui::Checkbox("##visible", &component.ShowBorder);
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

		DrawComponent<SpriteRendererComponent>("Sprite", entity, [](auto& entity, auto& component) {
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

		DrawComponent<CircleRendererComponent>("Circle", entity, [](auto& entity, auto& component) {
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Thickness");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Fade");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Color");
			ImGui::NextColumn();
			ImGui::DragFloat("##Thickness", &component.Thickness, 0.01f, 0.0f, 1000000.0f, "%.3f");
			ImGui::DragFloat("##Fade", &component.Fade, 0.001f, 0.0f, 1.0f, "%.4f");
			ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color));
			ImGui::Columns(1);
		});

		DrawComponent<ScriptComponent>("Script", entity, [this](auto& entity, auto& component) {
			bool classExists = ScriptEngine::EntityClassExists(component.ClassName);
			static char buffer[128];
			strcpy(buffer, component.ClassName.c_str());
			if (!classExists)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));
			if (ImGui::InputText("Class", buffer, sizeof(buffer)))
				component.ClassName = buffer;
			if (!classExists)
				ImGui::PopStyleColor();

			// Fields
			if (m_EditorContext->SceneState == SceneState::PLAY) {
				Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity);
				if (scriptInstance) {
					const auto& fields = scriptInstance->GetScriptClass()->GetFields();
					for (const auto& [name, field] : fields) {
						if (field.Type == ScriptFieldType::Float) {
							float data = scriptInstance->GetFieldValue<float>(name);
							if (ImGui::DragFloat(name.c_str(), &data)) {
								scriptInstance->SetFieldValue(name, data);
							}
						}
					}
				}
			} else {
				if (classExists) {
					Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.ClassName);
					const auto& fields = entityClass->GetFields();
					auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
					for (const auto& [name, field] : fields) {
						if (entityFields.find(name) != entityFields.end()) {
							ScriptFieldInstance& scriptField = entityFields.at(name);
							if (field.Type == ScriptFieldType::Float) {
								float data = scriptField.GetValue<float>();
								if (ImGui::DragFloat(name.c_str(), &data))
									scriptField.SetValue(data);
							}
						} else {
							if (field.Type == ScriptFieldType::Float) {
								float data = 0.0f;
								if (ImGui::DragFloat(name.c_str(), &data)) {
									ScriptFieldInstance& fieldInstance = entityFields[name];
									fieldInstance.Field = field;
									fieldInstance.SetValue(data);
								}
							}
						}
					}
				}
			}
		});

		DrawComponent<NativeScriptComponent>("Native Script", entity, [](auto& entity, auto& component) {
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 smallButtonSize = { lineHeight + 3.0f, lineHeight };
			ImVec2 largeButtonSize = { 100, lineHeight };

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Script");
			ImGui::NextColumn();

			if (ImGui::Button("Native Script", largeButtonSize)) {
				std::string filepath = FileDialogs::OpenFile("Native Script (*.h)\0*.h\0");
				if (!filepath.empty()) {
					std::filesystem::path scriptPath = std::filesystem::path(filepath); // warning this is not relative
					component.Filename = scriptPath.filename().string();
					component.Path = scriptPath.string();
					BindScriptToComponent(component, scriptPath.filename().stem().string());
				}
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path scriptPath = std::filesystem::path(g_AssetPath) / path;
					component.Filename = scriptPath.filename().string();
					component.Path = scriptPath.string();
					BindScriptToComponent(component, scriptPath.filename().stem().string());
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Text(component.Filename.c_str());

			ImGui::Columns(1);
		});

		DrawComponent<ScriptManagerComponent>("Script Manager", entity, [](auto& entity, auto& component) {
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 smallButtonSize = { lineHeight + 3.0f, lineHeight };
			ImVec2 largeButtonSize = { 100, lineHeight };

			DrawScriptManagerDropbox(component.NativeScripts, 0);
			for (int i = 0; i < component.NativeScripts.size(); i++) {
				ImGui::BeginChild(std::to_string(i).c_str(), {0, 25});
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 120.0f);
				std::string filename = component.NativeScripts[i].Filename;
				int extensionPos = filename.find_last_of(".");
				if (extensionPos != std::string::npos && extensionPos != 0 && extensionPos != filename.length() - 1)
					filename = filename.substr(0, extensionPos);
				ImGui::Dummy({ 0, 2 });
				ImGui::Text(filename.c_str());
				ImGui::NextColumn();

				if (ImGui::Button("Native Script", largeButtonSize)) {
					std::string filepath = FileDialogs::OpenFile("Native Script (*.h)\0*.h\0");
					if (!filepath.empty()) {
						std::filesystem::path scriptPath = std::filesystem::path(filepath); // warning this is not relative
						component.NativeScripts[i].Filename = scriptPath.filename().string();
						component.NativeScripts[i].Path = scriptPath.string();
						BindScriptToComponent(component.NativeScripts[i], scriptPath.filename().stem().string());
					}
				}
				if (ImGui::BeginDragDropSource()) {
					int index = i;
					ImGui::SetDragDropPayload("DRAG_SCRIPT", &index, sizeof(int));
					ImGui::EndDragDropSource();
				}
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path scriptPath = std::filesystem::path(g_AssetPath) / path;
						component.NativeScripts[i].Filename = scriptPath.filename().string();
						component.NativeScripts[i].Path = scriptPath.string();
						BindScriptToComponent(component.NativeScripts[i], scriptPath.filename().stem().string());
					}
					ImGui::EndDragDropTarget();
				}
				
				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 30);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::Button("X")) {
					component.NativeScripts.erase(component.NativeScripts.begin() + i);
				}
				ImGui::PopStyleColor();
				ImGui::Columns(1);
				ImGui::EndChild();
				DrawScriptManagerDropbox(component.NativeScripts, i + 1);
			}
			ImGui::Dummy({ 0, 10 });
			if (ImGui::Button("Add", { 60, lineHeight }))
				component.NativeScripts.emplace_back(NativeScriptComponent());
		});

		DrawComponent<RigidBody2DComponent>("Rigid Body 2D", entity, [](auto& entity, auto& component) {
			const char* bodyTypeString[] = { "Static", "Kinematic", "Dynamic"};
			const char* currentBodyTypeString = bodyTypeString[(int)component.Type];
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Body Type");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Fixed Rotation");
			ImGui::NextColumn();
			if (ImGui::BeginCombo("##bodytype", currentBodyTypeString)) {
				for (int i = 0; i < 3; i++) {
					bool isSelected = currentBodyTypeString == bodyTypeString[i];
					if (ImGui::Selectable(bodyTypeString[i], isSelected)) {
						currentBodyTypeString = bodyTypeString[i];
						component.Type = (RigidBody2DComponent::BodyType)i;
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Checkbox("##fixedrotation", &component.FixedRotation);
			ImGui::Columns(1);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& entity, auto& component) {
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			auto boldFont = io.Fonts->Fonts[0];
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 smallButtonSize = { lineHeight + 3.0f, lineHeight };
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Offset");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Size");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Density");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Friction");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Restitution");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Rest. Threshold");
			ImGui::NextColumn();

			ImGui::PushItemWidth(50);
			style.ItemSpacing = ImVec2(0, 0); // remove spacing

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", smallButtonSize))
				component.Offset.x = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Xoffset", &component.Offset.x, 0.1f, 1, 10000);
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", smallButtonSize))
				component.Offset.y = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Yoffset", &component.Offset.y, 0.1f, 1, 10000);
			ImGui::Dummy(ImVec2(0, linespacing));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("W", smallButtonSize))
				component.Size.x = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Woffset", &component.Size.x, 0.1f, 1, 10000);
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("H", smallButtonSize))
				component.Size.y = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Hoffset", &component.Size.y, 0.1f, 1, 10000);
			ImGui::Dummy(ImVec2(0, linespacing));

			ImGui::PopItemWidth();
			style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add back spacing

			ImGui::PushItemWidth(154);
			ImGui::DragFloat("##Density", &component.Density, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Friction", &component.Friction, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Restitution", &component.Restitution, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##RestitutionThreshold", &component.RestitutionThreshold, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::Columns(1);
		});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& entity, auto& component) {
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			auto boldFont = io.Fonts->Fonts[0];
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 smallButtonSize = { lineHeight + 3.0f, lineHeight };
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Offset");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Radius");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Density");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Friction");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Restitution");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Rest. Threshold");
			ImGui::NextColumn();

			ImGui::PushItemWidth(50);
			style.ItemSpacing = ImVec2(0, 0); // remove spacing

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", smallButtonSize))
				component.Offset.x = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Xoffset", &component.Offset.x, 0.1f, 1, 10000);
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", smallButtonSize))
				component.Offset.y = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Yoffset", &component.Offset.y, 0.1f, 1, 10000);
			ImGui::Dummy(ImVec2(0, linespacing));

			ImGui::PopItemWidth();
			style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add back spacing

			ImGui::PushItemWidth(154);
			ImGui::DragFloat("##Radius", &component.Radius, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Density", &component.Density, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Friction", &component.Friction, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Restitution", &component.Restitution, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##RestitutionThreshold", &component.RestitutionThreshold, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::Columns(1);
		});
	}
}